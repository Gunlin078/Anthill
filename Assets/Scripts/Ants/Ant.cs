using UnityEngine;

public abstract class Ant : MonoBehaviour, IDamageable
{
    [Header("Base Ant Settings")]
    public abstract AntType Type { get; }
    [SerializeField] protected float speed = 2f;
    [SerializeField] protected int maxHealth = 100;

    public int CurrentHealth => _currentHealth;
    protected int _currentHealth;

    protected virtual void Awake()
    {
        _currentHealth = maxHealth;
    }

    public void TakeDamage(int damage)
    {
        if (damage <= 0) return;

        _currentHealth -= damage;

        if (_currentHealth <= 0)
        {
            _currentHealth = 0;
            Die();
        }
    }

    protected virtual void Die()
    {
        Destroy(gameObject);
    }
}
