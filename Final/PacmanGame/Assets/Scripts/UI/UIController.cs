using UnityEngine;
using System.Collections;
using UnityEngine.UI;

public class UIController : MonoBehaviour
{
    public void OnLivesChanged(int newValue)
    {
        m_livesBar.SetValue(newValue);
    }

    public void OnScoreChanged(int newScore)
    {
        m_scoreText.text = string.Format("Score: {0:D4}", newScore);
    }

    public void OnHighscoreChanged(int newHighscore)
    {
        m_highscoreText.text = string.Format("High Score: {0:D4}", newHighscore);
    }

    [SerializeField]
    private Text m_scoreText;

    [SerializeField]
    private Text m_highscoreText;

    [SerializeField]
    private CounterBar m_livesBar;
}
