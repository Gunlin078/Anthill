using System.Collections;
using UnityEngine;

public abstract class Ant : MonoBehaviour, IDamageable
{
    [Header("Base Ant Settings")]
    [SerializeField] protected float speed = 2f;
    [SerializeField] protected int maxHealth = 100;

    [Header("Movement")]
    [SerializeField] protected float arriveDistance = 0.08f;
    [SerializeField] protected float slowdownRadius = 0.5f;
    [Tooltip("Вес расталкивания. 1.5–2 = муравьи обходят друг друга.")]
    [SerializeField] protected float separationWeight = 1.5f;

    public virtual AntColony Colony { get; protected set; }
    public int CurrentHealth => _currentHealth;
    protected int _currentHealth;
    protected bool _isInitializedCorrectly;

    // Movement state
    protected Vector2 _position;
    protected Vector2 _target;
    protected Vector2 _separation;
    protected bool _isMoving;

    public bool IsMoving => _isMoving;
    public Vector2 Position => _position;

    //                                              Unity functions

    protected virtual void Start()
    {
        if (!_isInitializedCorrectly || Colony == null)
        {
            Debug.LogError($"Error: {gameObject.name} was created via standard Instantiate! Use Ant.Spawn instead", this);
            gameObject.SetActive(false);
            Destroy(gameObject);
        }
    }

    protected virtual void OnEnable()
    {
        if (_isInitializedCorrectly)
        {
            ResetState();
            AntMovementSystem.Instance?.Register(this);
        }
    }

    protected virtual void OnDisable()
    {
        AntMovementSystem.Instance?.Unregister(this);
    }

    //                                              Custom functions

    public static T Spawn<T>(T prefab, AntColony colony) where T : Ant
    {
        if (prefab == null) return null;
        if (colony == null)
        {
            Debug.LogError($"[AntSpawn] Cannot spawn ant {prefab.name} without a valid AntColony!");
            return null;
        }

        T newAnt = Instantiate(prefab);
        newAnt.InternalInit(colony);
        return newAnt;
    }

    protected void InternalInit(AntColony colony)
    {
        Colony = colony;
        _isInitializedCorrectly = true;

        if (!this.enabled) this.enabled = true;
        else ResetState();

        // Регистрация именно здесь — OnEnable при Instantiate ещё не знает про colony
        AntMovementSystem.Instance?.Register(this);
    }

    protected virtual void ResetState()
    {
        _currentHealth = maxHealth;
        _position = Colony.transform.position;
        transform.position = _position;
        _target = _position;
        _separation = Vector2.zero;
        _isMoving = false;
    }

    //                                              Movement API

    /// <summary>Запустить движение к цели. Fire-and-forget.</summary>
    public void StartMove(Vector2 target)
    {
        _target = target;
        _isMoving = true;
    }

    /// <summary>Дождаться прибытия (если нужно). Не используй 500 штук разом — просто жди IsMoving.</summary>
    public IEnumerator MoveTo(Vector3 target)
    {
        StartMove(target);
        while (_isMoving) yield return null;
    }

    // Вызывается системой
    internal void ApplySeparation(Vector2 sep) => _separation = sep;

    internal void MovementTick(float dt)
    {
        // Ничего не делаем, если стоим и никто не толкает
        if (!_isMoving && _separation.sqrMagnitude < 0.0001f) return;

        Vector2 toTarget = _target - _position;
        float dist = toTarget.magnitude;
        bool atTarget = dist <= arriveDistance;

        Vector2 move = Vector2.zero;

        if (!atTarget)
        {
            Vector2 dir = toTarget / dist;
            float slowdown = Mathf.Clamp01(dist / slowdownRadius);
            move = dir * (speed * slowdown);
        }

        // Добавляем расталкивание (затухающее к цели)
        move += _separation * separationWeight;

        // Кэп по скорости — separation не должен «катапультировать» муравья
        float cap = speed;
        if (move.sqrMagnitude > cap * cap)
            move = move.normalized * cap;

        if (move.sqrMagnitude > 0.0000001f)
        {
            _position += move * dt;
            transform.position = _position;
        }

        // Достигли цели и никто не мешает — останавливаемся
        if (_isMoving && atTarget && _separation.sqrMagnitude < 0.04f)
            _isMoving = false;
    }

    //                                              Damage

    public void TakeDamage(int damage)
    {
        if (damage <= 0 || _currentHealth <= 0) return;
        _currentHealth -= damage;
        if (_currentHealth <= 0) { _currentHealth = 0; Die(); }
    }

    protected virtual void Die()
    {
        gameObject.SetActive(false);
        // возврат в пул — делай через Colony.DespawnAnt(...) — см. примечание ниже
    }
}