using UnityEngine;

public abstract class BaseAntData : ScriptableObject
{
    [SerializeField] private Sprite _sprite;
    [SerializeField] private float _maxHealth = 1;
    [SerializeField] private float _speed = 1;

    public Sprite Sprite => _sprite;
    public float MaxHealth => _maxHealth;
    public float Speed => _speed;
}
