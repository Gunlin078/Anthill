using UnityEngine;

public abstract class Ant : MonoBehaviour
{
    [Header("Base Ant Settings")]
    [SerializeField] protected float speed = 2f;
    [SerializeField] protected int maxHealth = 10;

    protected int currentHealth;

    protected virtual void Awake()
    {
        currentHealth = maxHealth;
    }

    public virtual void TakeDamage(int damage)
    {
        if (currentHealth <= 0 || damage <= 0) return;

        currentHealth -= damage;

        if (currentHealth <= 0)
        {
            currentHealth = 0;
            Die();
        }
    }


    protected virtual void Die()
    {
        Destroy(gameObject);
    }
}
