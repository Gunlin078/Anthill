using System;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Pool;

public class AntManager : MonoBehaviour
{
    [Header("Префабы муравьев")]
    [SerializeField] private Ant workerPrefab;
    [SerializeField] private Ant warriorPrefab;

    [Header("Настройки пулинга")]
    [SerializeField] private int defaultCapacity = 20;
    [SerializeField] private int absoluteMaxSize = 5000;

    [Header("Динамический игровой лимит")]
    [SerializeField] private int maxCurrentPoolSize = 50;

    private Dictionary<AntType, ObjectPool<Ant>> _pools;

    private Dictionary<AntType, int> _activeCounts = new();

    private void Awake()
    {
        _pools = new Dictionary<AntType, ObjectPool<Ant>>();

        foreach (AntType type in Enum.GetValues(typeof(AntType)))
        {
            _activeCounts[type] = 0;
        }

        _pools[AntType.Worker] = CreatePoolForType(AntType.Worker, workerPrefab);
        _pools[AntType.Warrior] = CreatePoolForType(AntType.Warrior, warriorPrefab);
    }

    private ObjectPool<Ant> CreatePoolForType(AntType type, Ant prefab)
    {
        return new ObjectPool<Ant>(
            createFunc: () => Instantiate(prefab, transform),
            actionOnGet: (ant) => ant.gameObject.SetActive(true),
            actionOnRelease: (ant) => {
                ant.ResetState();
                ant.gameObject.SetActive(false);
            },
            actionOnDestroy: (ant) => Destroy(ant.gameObject),
            collectionCheck: true,
            defaultCapacity: defaultCapacity,
            maxSize: absoluteMaxSize
        );
    }

    /// <summary>
    /// Изменить разрешенный максимум муравьев во время игры (например, при апгрейде)
    /// </summary>
    public void SetMaxCurrentPoolSize(int newLimit)
    {
        maxCurrentPoolSize = newLimit;
        Debug.Log($"[AntManager] Игровой лимит муравьев увеличен до: {maxCurrentPoolSize}");
    }

    // ==========================================
    // Public API с учетом лимита
    // ==========================================

    public Ant GetAnt(AntType type)
    {
        // Проверяем, не достигли ли мы ТЕКУЩЕГО игрового лимита
        if (_activeCounts[type] >= maxCurrentPoolSize)
        {
            Debug.LogWarning($"[AntManager] Нельзя создать {type}. Достигнут текущий лимит: {maxCurrentPoolSize}");
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
            _activeCounts[ant.Type]--; // Уменьшаем счетчик активных, освобождая место

            // Защита от ухода счетчика в минус (на всякий случай)
            if (_activeCounts[ant.Type] < 0) _activeCounts[ant.Type] = 0;
        }
    }
}
