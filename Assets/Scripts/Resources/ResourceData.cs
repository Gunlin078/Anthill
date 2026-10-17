using UnityEngine;

[CreateAssetMenu(fileName = "ResourceData", menuName = "Scriptable Objects/ResourceData")]
public class ResourceData : ScriptableObject
{
    public string resourceName;       
    public Sprite icon;               
    private int _capacity = 99;
}
