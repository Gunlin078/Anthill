using System.Collections;
using UnityEngine;

public abstract class Ant : MonoBehaviour, IDamageable
{
    [Header("Base Ant Settings")]
    [SerializeField] protected float _speed = 2f;              // in Data
    [SerializeField] protected int _maxHealth = 100;           // in Data

    [Header("Movement")]
    [SerializeField] protected float _arriveDistance = 0.08f;  // in Data
    [SerializeField] protected float _slowdownRadius = 0.5f;   // in Data
    [Tooltip("Вес расталкивания. 1.5–2 = муравьи обходят друг друга.")]
    [SerializeField] protected float _separationWeight = 1.5f; // in Data

    protected Vector2 _position;
    protected Vector2 _target;
    protected Vector2 _separation;
    protected bool _isMoving;
    public bool IsMoving => _isMoving;
    public Vector2 Position => _position;

    public virtual AntColony Colony { get; protected set; }
    public AntType Type { get; protected set; }
    public int CurrentHealth => _currentHealth;
    protected int _currentHealth;

    protected bool _isInitializedCorrectly;



    //                                                            Unity functions



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
        if (!_isInitializedCorrectly) return;

        ResetState();
        AntMovementSystem.Instance.Register(this);
    }

    protected virtual void OnDisable()
    {
        AntMovementSystem.Instance.Unregister(this);
    }


    //                                              Spawn / Init


    public static T Spawn<T>(T prefab, AntColony colony, AntType type) where T : Ant
    {
        if (prefab == null) return null;
        if (colony == null)
        {
            Debug.LogError($"[AntSpawn] Cannot spawn ant {prefab.name} without a valid AntColony!");
            return null;
        }

        T newAnt = Instantiate(prefab);
        newAnt.InternalInit(colony, type);
        return newAnt;
    }

    protected void InternalInit(AntColony colony, AntType type)
    {
        Colony = colony;
        Type = type;
        _isInitializedCorrectly = true;
        ResetState();

        if (gameObject.activeInHierarchy)
            AntMovementSystem.Instance.Register(this);
    }

    protected virtual void ResetState()
    {
        _currentHealth = _maxHealth;
        _position = Colony != null ? (Vector2)Colony.transform.position : (Vector2)transform.position;
        transform.position = _position;
        _target = _position;
        _separation = Vector2.zero;
        _isMoving = false;
    }

    //                                              Movement API

    public void StartMove(Vector2 target)
    {
        _target = target;
        _isMoving = true;
    }

    public void StopMove()
    {
        _isMoving = false;
        _target = _position;
    }

    internal void ApplySeparation(Vector2 sep) => _separation = sep;

    internal void MovementTick(float dt)
    {
        if (!_isMoving && _separation.sqrMagnitude < 0.0001f) return;

        Vector2 toTarget = _target - _position;
        float dist = toTarget.magnitude;
        bool atTarget = dist <= _arriveDistance;

        Vector2 move = Vector2.zero;

        if (!atTarget)
        {
            Vector2 dir = toTarget / dist;
            float slowdown = Mathf.Clamp01(dist / _slowdownRadius);
            move = dir * (_speed * slowdown);
        }

        move += _separation * _separationWeight;

        float cap = _speed;
        if (move.sqrMagnitude > cap * cap)
            move = move.normalized * cap;

        if (move.sqrMagnitude > 1e-7f)
        {
            _position += move * dt;
            transform.position = _position;
        }

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
        if (Colony != null)
            Colony.DespawnAnt(Type, this);
        else
            gameObject.SetActive(false);
    }
}