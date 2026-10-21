using System.Collections.Generic;
using UnityEngine;

[DefaultExecutionOrder(-1000)] // Awake раньше AntColony, чтобы Instance уже был
public sealed class AntMovementSystem : MonoBehaviour
{
    public static AntMovementSystem Instance { get; private set; }

    [Header("Separation")]
    [Tooltip("Радиус расталкивания. Ставь ~= диаметру спрайта + запас.")]
    [SerializeField] private float separationRadius = 0.5f;

    [Tooltip("Как часто пересчитывать расталкивание (сек). 0.05 = 20 Гц.")]
    [SerializeField] private float separationInterval = 0.05f;

    [Tooltip("true = FixedUpdate (стабильнее), false = Update (плавнее).")]
    [SerializeField] private bool tickInFixedUpdate = true;

    private readonly List<Ant> _ants = new(512);
    private readonly Dictionary<long, List<Ant>> _grid = new(1024);
    private readonly Stack<List<Ant>> _listPool = new(128);

    private float _cellSize;
    private float _nextSepTime;

    private void Awake()
    {
        if (Instance != null && Instance != this) { Destroy(gameObject); return; }
        Instance = this;
        _cellSize = Mathf.Max(0.01f, separationRadius);
    }

    private void OnDestroy()
    {
        if (Instance == this) Instance = null;
    }

    public void Register(Ant ant)
    {
        if (!_ants.Contains(ant)) _ants.Add(ant);
    }

    public void Unregister(Ant ant)
    {
        _ants.Remove(ant);
    }

    private void Update()
    {
        if (!tickInFixedUpdate) Tick(Time.deltaTime);
    }

    private void FixedUpdate()
    {
        if (tickInFixedUpdate) Tick(Time.fixedDeltaTime);
    }

    private void Tick(float dt)
    {
        int n = _ants.Count;
        if (n == 0) return;

        // Separation — с троттлингом
        if (Time.time >= _nextSepTime)
        {
            _nextSepTime = Time.time + separationInterval;
            RebuildGrid();

            for (int i = 0; i < n; i++)
                _ants[i].ApplySeparation(ComputeSeparation(_ants[i]));
        }

        // Движение — каждый тик
        for (int i = 0; i < n; i++)
            _ants[i].MovementTick(dt);
    }

    private void RebuildGrid()
    {
        // Возвращаем List'ы в пул — без аллокаций
        foreach (var list in _grid.Values)
        {
            list.Clear();
            _listPool.Push(list);
        }
        _grid.Clear();

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
                cell = _listPool.Count > 0 ? _listPool.Pop() : new List<Ant>(8);
                _grid[key] = cell;
            }
            cell.Add(ant);
        }
    }

    private Vector2 ComputeSeparation(Ant self)
    {
        Vector2 pos = self.Position;
        int cx = Mathf.FloorToInt(pos.x / _cellSize);
        int cy = Mathf.FloorToInt(pos.y / _cellSize);
        float sqrR = separationRadius * separationRadius;

        Vector2 push = Vector2.zero;

        for (int ox = -1; ox <= 1; ox++)
            for (int oy = -1; oy <= 1; oy++)
            {
                if (!_grid.TryGetValue(Key(cx + ox, cy + oy), out var cell)) continue;

                for (int i = 0; i < cell.Count; i++)
                {
                    var other = cell[i];
                    if (other == self) continue;

                    Vector2 d = pos - other.Position;
                    float sqr = d.sqrMagnitude;
                    if (sqr > sqrR || sqr < 0.000001f) continue;

                    float dist = Mathf.Sqrt(sqr);
                    // push = направление * (1 - d/R) / d  →  плавное затухание
                    push += d * ((1f - dist / separationRadius) / dist);
                }
            }
        return push;
    }

    private static long Key(int x, int y) => ((long)x << 32) ^ (uint)y;
}