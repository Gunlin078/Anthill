using UnityEngine;

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
