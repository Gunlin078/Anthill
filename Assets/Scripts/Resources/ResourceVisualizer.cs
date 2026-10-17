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


[RequireComponent(typeof(SpriteRenderer))]
public class ResourceVisualizer : MonoBehaviour
{
    [SerializeField] private ResourceData _resourceData;

    private SpriteRenderer _spriteRenderer;

    private void Awake()
    {
        _spriteRenderer = GetComponent<SpriteRenderer>();
    }

    private void Start()
    {
        if (_resourceData != null && _resourceData.Sprite != null)
        {
            _spriteRenderer.sprite = _resourceData.Sprite;
        }
    }
}
