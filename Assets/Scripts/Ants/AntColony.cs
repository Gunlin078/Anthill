using System;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Pool;

public class AntColony : MonoBehaviour
{
    [Header("Prefabs")]
    [SerializeField] private Ant _workerPrefab;
    [SerializeField] private Ant _warriorPrefab;

    [Header("Pool Settings")]
    [SerializeField] private int _defaultCapacity = 10;
    [SerializeField] private int _maxPoolSize = 100;

    [Header("Current ant limit")]
    [SerializeField] private int _currentMaxPoolSize = 50;

    private Dictionary<AntType, ObjectPool<Ant>> _pools;
    private Dictionary<AntType, int> _activeCounts = new Dictionary<AntType, int>();

    private void Awake()
    {
        _pools = new Dictionary<AntType, ObjectPool<Ant>>();

        foreach (AntType type in Enum.GetValues(typeof(AntType)))
        {
            _activeCounts[type] = 0;
        }

        _pools[AntType.Worker]  = CreatePoolForType(_workerPrefab);
        _pools[AntType.Warrior] = CreatePoolForType(_warriorPrefab);
    }

    private ObjectPool<Ant> CreatePoolForType(Ant prefab)
    {
        return new ObjectPool<Ant>(
            createFunc: () => Ant.Spawn(prefab, this),
            actionOnGet:     (ant) => ant.gameObject.SetActive(true),
            actionOnRelease: (ant) => ant.gameObject.SetActive(false),
            actionOnDestroy: (ant) => Destroy(ant.gameObject),
            collectionCheck: true,
            defaultCapacity: _defaultCapacity,
            maxSize:         _maxPoolSize
        );
    }

    /// <summary>
    /// Change the maximum allowed number of ants during the game
    /// </summary>
    public void SetMaxCurrentPoolSize(int newLimit)
    {
        if (newLimit > _maxPoolSize)
        {
            Debug.Log($"[AntColony] The desired limit exceeds N: {_maxPoolSize}");
        }
        
        _currentMaxPoolSize = newLimit;
        Debug.Log($"[AntColony] The current colony ant limit has been increased to {_currentMaxPoolSize} (Max capacity: {_maxPoolSize})");
    }

    public Ant GetAnt(AntType type)
    {
        if (_activeCounts[type] >= _currentMaxPoolSize)
        {
            Debug.LogWarning($"[AntColony] Cannot create {type}. Current limit: {_currentMaxPoolSize}");
            return null;
        }

        if (_pools.TryGetValue(type, out var pool))
        {
            Ant ant = pool.Get();
            _activeCounts[type]++;
            return ant;
        }
        return null;
    }

    public void DespawnAnt(AntType type, Ant ant)
    {
        if (_pools.TryGetValue(type, out var pool))
        {
            pool.Release(ant);
            _activeCounts[type] = Mathf.Max(0, _activeCounts[type] - 1);
        }
    }

    private void OnDestroy()
    {
        if (_pools == null) return;

        foreach (var pool in _pools.Values)
        {
            pool.Clear();
        }
        _pools.Clear();
    }

    public int GetActiveAntCount(AntType type)
    {
        return _activeCounts.TryGetValue(type, out int count) ? count : 0;
    }


}
