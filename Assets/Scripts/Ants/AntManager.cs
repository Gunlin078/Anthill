using System;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Pool;

public class AntManager : MonoBehaviour
{
    [Header("Префабы муравьев")]
    [SerializeField] private Ant _workerPrefab;
    [SerializeField] private Ant _warriorPrefab;

    [Header("Настройки пулинга")]
    [SerializeField] private int _defaultCapacity = 20;
    [SerializeField] private int _absoluteMaxSize = 5000;

    [Header("Динамический игровой лимит")]
    [SerializeField] private int _maxCurrentPoolSize = 50;

    private Dictionary<AntType, ObjectPool<Ant>> _pools;

    private Dictionary<AntType, int> _activeCounts = new();

    private void Awake()
    {
        _pools = new Dictionary<AntType, ObjectPool<Ant>>();

        foreach (AntType type in Enum.GetValues(typeof(AntType)))
        {
            _activeCounts[type] = 0;
        }

        _pools[AntType.Worker] = CreatePoolForType(AntType.Worker, _workerPrefab);
        _pools[AntType.Warrior] = CreatePoolForType(AntType.Warrior, _warriorPrefab);
    }

    private ObjectPool<Ant> CreatePoolForType(AntType type, Ant prefab)
    {
        return new ObjectPool<Ant>(
            createFunc: () => Instantiate(prefab, transform),
            actionOnGet: (ant) => ant.Spawn(),
            actionOnRelease: (ant) => ant.TurnOff(),
            actionOnDestroy: (ant) => Destroy(ant.gameObject),
            collectionCheck: true,
            defaultCapacity: _defaultCapacity,
            maxSize: _absoluteMaxSize
        );
    }

    /// <summary>
    /// Change the maximum allowed number of ants during the game
    /// </summary>
    public void SetMaxCurrentPoolSize(int newLimit)
    {
        _maxCurrentPoolSize = newLimit;
        Debug.Log($"[AntManager] Игровой лимит муравьев увеличен до: {_maxCurrentPoolSize}");
    }

    public Ant GetAnt(AntType type)
    {
        // Проверяем, не достигли ли мы ТЕКУЩЕГО игрового лимита
        if (_activeCounts[type] >= _maxCurrentPoolSize)
        {
            Debug.LogWarning($"[AntManager] Нельзя создать {type}. Достигнут текущий лимит: {_maxCurrentPoolSize}");
            return null; // Возвращаем null, игра понимает, что лимит исчерпан
        }

        if (_pools.TryGetValue(type, out var pool))
        {
            Ant ant = pool.Get();
            _activeCounts[type]++; // Увеличиваем счетчик активных на сцене
            return ant;
        }
        return null;
    }

    public void ReturnAnt(Ant ant)
    {
        if (ant == null) return;

        if (_pools.TryGetValue(ant.Type, out var pool))
        {
            pool.Release(ant);
            _activeCounts[ant.Type]--;

            if (_activeCounts[ant.Type] < 0) _activeCounts[ant.Type] = 0;
        }
    }
}
