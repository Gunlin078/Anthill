using UnityEngine;

public abstract class Ant : MonoBehaviour, IDamageable
{
    [Header("Base Ant Settings")]
    [SerializeField] protected float speed = 2f;
    [SerializeField] protected int maxHealth = 100;

    public virtual AntColony Colony { get; protected set; } 

    public int CurrentHealth => _currentHealth;
    protected int _currentHealth;

    protected bool _isInitializedCorrectly;


    //                                              Unity functions

    protected virtual void Start()
    {
        if (!_isInitializedCorrectly || Colony == null)
        {
            Debug.LogError($"Error: {gameObject.name} was created via standard Instantiate or placed on scene! Use Ant.Spawn instead", this);
            gameObject.SetActive(false);
            Destroy(gameObject);
        }
    }

    protected virtual void OnEnable()
    {
        if (_isInitializedCorrectly)
        {
            ResetState();
        }
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

        if (!this.enabled)  {   this.enabled = true; }
        else                {   ResetState();        }
    }

    protected virtual void ResetState()
    {
        _currentHealth = maxHealth;
        transform.position = Colony.transform.position;
    }

    public void TakeDamage(int damage)
    {
        if (damage <= 0 || _currentHealth <= 0) return;

        _currentHealth -= damage;

        if (_currentHealth <= 0)
        {
            _currentHealth = 0;
            Die();
        }
    }

    protected virtual void Die()
    {
        gameObject.SetActive(false);
        //возврат в пул
    }
}
