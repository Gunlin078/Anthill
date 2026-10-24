using System.Collections.Generic;
using UnityEngine;

[DefaultExecutionOrder(-1000)]
public sealed class AntMovementSystem : MonoBehaviour
{
    public static AntMovementSystem Instance { get; private set; }

    [Header("Separation")]
    [Tooltip("Радиус взаимного отталкивания.")]
    [SerializeField] private float _separationRadius = 0.5f;

    [Tooltip("Как часто пересчитывать направление расталкивания.")]
    [SerializeField] private float _separationInterval = 0.05f;

    [Tooltip("За сколько секунд сила плавно доезжает до новой цели. ~50% от interval.")]
    [SerializeField] private float _separationSmoothing = 0.025f;

    [Tooltip("Максимум модуля силы. Без clamp в плотной куче push улетает.")]
    [SerializeField] private float _maxSeparationForce = 1.5f;

    [SerializeField] private bool _tickInFixedUpdate = true;

    private readonly List<Ant> _ants = new(1024);
    private readonly HashSet<Ant> _antSet = new(1024);

    private readonly Dictionary<long, List<Ant>> _grid = new(2048);
    private readonly List<long> _activeKeys = new(2048);
    private readonly Stack<List<Ant>> _listPool = new(256);

    private readonly Dictionary<Ant, Vector2> _currentPush = new(1024);
    private readonly Dictionary<Ant, Vector2> _targetPush = new(1024);

    private float _cellSize;
    private float _nextSepTime;

    private void Awake()
    {
        if (Instance != null && Instance != this)
        {
            Destroy(gameObject);
            return;
        }
        Instance = this;
        RefreshCellSize();
        _nextSepTime = 0f;
    }

    private void OnValidate() => RefreshCellSize();

    private void RefreshCellSize() => _cellSize = Mathf.Max(0.01f, _separationRadius);

    private void OnDestroy()
    {
        if (Instance == this) Instance = null;

        _ants.Clear();
        _antSet.Clear();
        _grid.Clear();
        _activeKeys.Clear();
        _listPool.Clear();
        _currentPush.Clear();
        _targetPush.Clear();
    }

    public void Register(Ant ant)
    {
        if (ant == null || !_antSet.Add(ant)) return;
        _ants.Add(ant);
        _currentPush[ant] = Vector2.zero;
        _targetPush[ant] = Vector2.zero;
    }

    public void Unregister(Ant ant)
    {
        if (ant == null || !_antSet.Remove(ant)) return;
        _ants.Remove(ant);
        _currentPush.Remove(ant);
        _targetPush.Remove(ant);
    }

    private void Update()
    {
        if (!_tickInFixedUpdate) Tick(Time.deltaTime, Time.time);
    }

    private void FixedUpdate()
    {
        if (_tickInFixedUpdate) Tick(Time.fixedDeltaTime, Time.fixedTime);
    }

    private void Tick(float dt, float currentTime)
    {
        int n = _ants.Count;
        if (n == 0) return;

        // 1) Редко: пересчитываем ЦЕЛЕВУЮ силу.
        if (currentTime >= _nextSepTime)
        {
            _nextSepTime = currentTime + Mathf.Max(0.0001f, _separationInterval);

            RebuildGrid();
            for (int i = 0; i < n; i++)
                _targetPush[_ants[i]] = ComputeSeparation(_ants[i]);
        }

        // 2) Каждый кадр: сглаживаем текущую силу к целевой и двигаем.
        bool smooth = _separationSmoothing > 0.0001f;
        float maxDelta = smooth ? (_maxSeparationForce / _separationSmoothing) * dt : 0f;

        for (int i = 0; i < n; i++)
        {
            var ant = _ants[i];

            Vector2 current = _currentPush.TryGetValue(ant, out var c) ? c : Vector2.zero;
            Vector2 target = _targetPush.TryGetValue(ant, out var t) ? t : Vector2.zero;

            Vector2 smoothed = smooth ? Vector2.MoveTowards(current, target, maxDelta) : target;

            _currentPush[ant] = smoothed;
            ant.ApplySeparation(smoothed);
            ant.MovementTick(dt);
        }
    }

    private void RebuildGrid()
    {
        int activeCount = _activeKeys.Count;
        for (int i = 0; i < activeCount; i++)
        {
            if (_grid.TryGetValue(_activeKeys[i], out var list))
            {
                list.Clear();
                _listPool.Push(list);
            }
        }
        _grid.Clear();
        _activeKeys.Clear();

        int n = _ants.Count;
        for (int i = 0; i < n; i++)
        {
            var ant = _ants[i];
            Vector2 p = ant.Position;
            int cx = Mathf.FloorToInt(p.x / _cellSize);
            int cy = Mathf.FloorToInt(p.y / _cellSize);
            long key = Key(cx, cy);

            if (!_grid.TryGetValue(key, out var cell))
            {
                cell = _listPool.Count > 0 ? _listPool.Pop() : new List<Ant>(16);
                _grid[key] = cell;
                _activeKeys.Add(key);
            }
            cell.Add(ant);
        }
    }

    private Vector2 ComputeSeparation(Ant self)
    {
        Vector2 pos = self.Position;
        int cx = Mathf.FloorToInt(pos.x / _cellSize);
        int cy = Mathf.FloorToInt(pos.y / _cellSize);
        float sqrR = _separationRadius * _separationRadius;
        Vector2 push = Vector2.zero;

        EntityId selfId = self.GetEntityId();

        for (int ox = -1; ox <= 1; ox++)
        {
            for (int oy = -1; oy <= 1; oy++)
            {
                if (!_grid.TryGetValue(Key(cx + ox, cy + oy), out var cell)) continue;

                int cellCount = cell.Count;
                for (int i = 0; i < cellCount; i++)
                {
                    var other = cell[i];
                    if (other == self) continue;

                    Vector2 d = pos - other.Position;
                    float sqr = d.sqrMagnitude;

                    if (sqr < 1e-6f)
                    {
                        float dir = selfId.CompareTo(other.GetEntityId()) < 0 ? 1f : -1f;
                        push += new Vector2(dir, 0f) * 0.1f;
                        continue;
                    }
                    if (sqr > sqrR) continue;

                    float dist = Mathf.Sqrt(sqr);
                    push += d * ((1f - dist / _separationRadius) / dist);
                }
            }
        }

        float m2 = push.sqrMagnitude;
        float max2 = _maxSeparationForce * _maxSeparationForce;
        if (m2 > max2 && m2 > 0f)
            push *= _maxSeparationForce / Mathf.Sqrt(m2);

        return push;
    }

    private static long Key(int x, int y) => ((long)x << 32) | (uint)y;
}