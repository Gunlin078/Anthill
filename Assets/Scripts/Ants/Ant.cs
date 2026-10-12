using UnityEngine;

public abstract class Ant : MonoBehaviour, IDamageable
{
    [Header("Base Ant Settings")]
    public abstract AntType Type { get; }
    [SerializeField] protected float speed = 2f;
    [SerializeField] protected int maxHealth = 100;

    public AntColony Colony { get; private set; }
    public int CurrentHealth => _currentHealth;
    protected int _currentHealth;

    protected void Spawn(AntColony colony, Vector2 spawnPosition)
    {
        Colony = colony;
        transform.position = spawnPosition;
        _currentHealth = maxHealth;
    }

    public void TakeDamage(int damage)
    {
        if (damage <= 0) return;

        _currentHealth -= damage;

        if (_currentHealth <= 0)
        {
            _currentHealth = 0;
            TurnOff();
        }
    }

    public virtual void TurnOff()
    {
        Colony = null;
        transform.localPosition = Vector2.zero;
        this.enabled = false;
    }
}
