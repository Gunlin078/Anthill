using UnityEngine.SceneManagement;
using UnityEngine;

/// <summary>
/// Global game manager. Implemented as a Singleton
/// </summary>
public class GameManager : MonoBehaviour
{
    public static GameManager Instance { get; private set; }

    public enum GameState { MainMenu, Gameplay, Pause }
    public GameState CurrentState { get; private set; }

    private void Awake()
    {
        if (Instance == null)
        {
            Instance = this;
            DontDestroyOnLoad(gameObject);
        }
        else
        {
            Destroy(gameObject);
        }
    }

    private void Start()
    {
        ChangeState(GameState.Gameplay);
    }

    public void ChangeState(GameState newState)
    {
        CurrentState = newState;

        switch (CurrentState)
        {
            case GameState.Gameplay:
                Time.timeScale = 1f; // Запуск времени игры
                break;
            case GameState.Pause:
                Time.timeScale = 0f; // Остановка времени (пауза)
                break;
            case GameState.GameOver:
                Time.timeScale = 0f;
                Debug.Log("Игра окончена! Показываем UI проигрыша.");
                break;
        }
    }

    public void RestartGame()
    {
        // Перезагрузка текущей сцены
        SceneManager.LoadScene(SceneManager.GetActiveScene().buildIndex);
    }
}
