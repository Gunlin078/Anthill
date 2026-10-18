using System.Collections;
using UnityEngine.AI;
using UnityEngine;

public class WorkerAnt : Ant
{
    private Coroutine _resourceLoopCoroutine;
    private NavMeshAgent _agent;
    private ResourceType _resourceType;
    /*
    private void StartResourceLoop(Vector2 targetPos)
    {
        if (_resourceLoopCoroutine == null)
        {
            _resourceLoopCoroutine = StartCoroutine(ResourceLoop(targetPos));
        }
    }
    
    private IEnumerator ResourceLoop(Vector2 targetPos)
    {
        return y;
    }
    */
}
