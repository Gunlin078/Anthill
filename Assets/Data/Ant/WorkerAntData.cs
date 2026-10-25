using UnityEngine;

[CreateAssetMenu(fileName = "WorkerAntData", menuName = "Scriptable Objects/WorkerAntData")]
public class WorkerAntData : BaseAntData
{
    [SerializeField] private float _extractionRate;
}
