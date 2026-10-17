using System.Collections;
using UnityEngine.AI;
using UnityEngine;

public class WorkerAnt : Ant
{
    private Coroutine resourceLoopCoroutine;
    private NavMeshAgent agent;

    private void StartResourceLoop(Vector2 targetPos)
    {
        if (resourceLoopCoroutine == null)
        {
            resourceLoopCoroutine = StartCoroutine(ResourceLoop(targetPos));
        }
    }
    private IEnumerator ResourceLoop(Vector2 targetPos)
    {
        return y;
    }
}
