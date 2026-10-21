using UnityEngine;

[CreateAssetMenu(fileName = "ResourceData", menuName = "Scriptable Objects/ResourceData")]
public class ResourceData : ScriptableObject
{
    [SerializeField] private ResourceType _type;
    [SerializeField] private Sprite _sprite;
    [SerializeField] private int _capacity = 99;

    public ResourceType Type => _type;
    public Sprite Sprite => _sprite;
    public int Capacity => _capacity;
}
