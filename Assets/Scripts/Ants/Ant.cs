using UnityEngine;

public abstract class Ant : MonoBehaviour , IDamageable , IInitializable
{
    [Header("Base Ant Settings")]
    [SerializeField] protected float speed = 2f;
    [SerializeField] protected int maxHealth = 100;

    protected int _currentHealth;
    public int CurrentHealth => _currentHealth;

    void IInitializable.Initialize()
    {
        //if (isSpawned) break
        _currentHealth = maxHealth;
    }

    public void TakeDamage(int damage)
    {
        if (CurrentHealth <= 0 || damage <= 0) return;

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
