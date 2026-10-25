using System;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Pool;

public class AntColony : MonoBehaviour
{
    [Header("Prefabs (Must be INACTIVE!)")]
    [SerializeField] private Ant _workerPrefab;
    [SerializeField] private Ant _warriorPrefab;

    [Header("Pool Settings")]
    [SerializeField] private int _defaultCapacity = 10;
    [SerializeField] private int _maxPoolSize = 100;

    [Header("Current ant limit")]
    [SerializeField] private int _currentMaxPoolSize = 20;

    private Dictionary<AntType, ObjectPool<Ant>> _pools;
    private Dictionary<AntType, int> _activeCounts = new();

    private void Awake()
    {
        _pools = new Dictionary<AntType, ObjectPool<Ant>>();

        foreach (AntType type in Enum.GetValues(typeof(AntType)))
            _activeCounts[type] = 0;

        _pools[AntType.Worker] = CreatePoolForType(_workerPrefab, AntType.Worker);
        _pools[AntType.Warrior] = CreatePoolForType(_warriorPrefab, AntType.Warrior);
    }

    private ObjectPool<Ant> CreatePoolForType(Ant prefab, AntType type)
    {
        if (prefab == null)
        {
            Debug.LogWarning($"[AntColony] Pool for {type} has no prefab assigned");
            return null;
        }

        return new ObjectPool<Ant>(
            createFunc: () =>
            {
                var ant = Ant.Spawn(prefab, this, type);
                if (ant != null && ant.gameObject.activeSelf)
                    ant.gameObject.SetActive(false);
                return ant;
            },
            actionOnGet: ant => ant.gameObject.SetActive(true),
            actionOnRelease: ant => ant.gameObject.SetActive(false),
            actionOnDestroy: ant => { if (ant != null) Destroy(ant.gameObject); },
            collectionCheck: true,
            defaultCapacity: _defaultCapacity,
            maxSize: _maxPoolSize
        );
    }

    public void SetMaxCurrentPoolSize(int newLimit)
    {
        if (newLimit > _maxPoolSize)
            Debug.LogWarning($"[AntColony] The desired limit exceeds {_maxPoolSize}");
        _currentMaxPoolSize = newLimit;
    }

    public Ant GetAnt(AntType type)
    {
        if (!_pools.TryGetValue(type, out var pool) || pool == null) return null;

        if (_activeCounts[type] >= _currentMaxPoolSize)
        {
            Debug.LogWarning($"[AntColony] Cannot create {type}. Current limit: {_currentMaxPoolSize}");
            return null;
        }

        Ant ant = pool.Get();
        if (ant == null) return null;
        _activeCounts[type]++;
        return ant;
    }

    public void DespawnAnt(AntType type, Ant ant)
    {
        if (ant == null) return;

        if (!ant.gameObject.activeSelf) return;

        if (_pools.TryGetValue(type, out var pool) && pool != null)
        {
            pool.Release(ant);
            _activeCounts[type] = Mathf.Max(0, _activeCounts[type] - 1);
        }
        else
        {
            ant.gameObject.SetActive(false);
        }
    }

    public int GetActiveAntCount(AntType type)
        => _activeCounts.TryGetValue(type, out int c) ? c : 0;

    private void OnDestroy()
    {
        if (_pools == null) return;
        foreach (var pool in _pools.Values) pool?.Clear();
        _pools.Clear();
    }
}