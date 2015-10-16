using System;
using UnityEngine;
using System.Collections;
using System.IO;
using UnityEngine.Events;

[Serializable]
public class ScoreChangedEvent : UnityEvent<int>{}

public class Game : MonoBehaviour
{
    public static Game Instance
    {
        get { return s_instance; }
    }

    public int Score
    {
        get { return m_score; }
    }

    public float FrightenTime
    {
        get { return 6.0f; }
    }

    public AgentInfo PacmanAgentInfo
    {
        get { return m_pacmanAgentInfo; }
    }

    public AgentInfo BlinkyAgentInfo
    {
        get { return m_blinkyAgentInfo; }
    }

    public AgentInfo PinkyAgentInfo
    {
        get { return m_pinkyAgentInfo; }
    }

    public AgentInfo InkyAgentInfo
    {
        get { return m_inkyAgentInfo; }
    }

    public AgentInfo ClydeAgentInfo
    {
        get { return m_clydeAgentInfo; }
    }

    void Awake()
    {
        s_instance = this;

        m_audioSource = GetComponent<AudioSource>();
    }

    void Start()
    {
        m_onScoreChanged.Invoke(m_score);

        if (File.Exists("Highscore.txt"))
        {
            using (FileStream fstream = new FileStream("Highscore.txt", FileMode.Open, FileAccess.Read))
            {
                using (StreamReader reader = new StreamReader(fstream))
                {
                    int.TryParse(reader.ReadLine(), out m_highScore);
                }
            }
        }

        m_onHighScoreChanged.Invoke(m_highScore);
    }

    void OnDestroy()
    {
        using (FileStream fstream = new FileStream("Highscore.txt", FileMode.OpenOrCreate, FileAccess.Write))
        {
            using (StreamWriter writer = new StreamWriter(fstream))
            {
                writer.Write(m_highScore.ToString());
            }
        }
    }

    void FixedUpdate()
    {
        lock (this)
        {
            m_pacmanAgentInfo = FindObjectOfType<PacmanAgent>().Info;
            m_blinkyAgentInfo = FindObjectOfType<BlinkyAgent>().Info;
            m_pinkyAgentInfo = FindObjectOfType<PinkyAgent>().Info;
            m_inkyAgentInfo = FindObjectOfType<InkyAgent>().Info;
            m_clydeAgentInfo = FindObjectOfType<ClydeAgent>().Info;
        }

        GhostAgent.State highestState = GhostAgent.State.Normal;

        GhostAgent[] ghosts = FindObjectsOfType<GhostAgent>();
        foreach (GhostAgent ghostAgent in ghosts)
        {
            if (ghostAgent.CurrentState != GhostAgent.State.Waiting && ghostAgent.CurrentState > highestState)
                highestState = ghostAgent.CurrentState;
        }

        if (highestState != globalState)
        {
            switch (highestState)
            {
                case GhostAgent.State.Normal:
                    if (m_audioSource.clip != m_ghostAudioClip)
                    {
                        m_audioSource.clip = m_ghostAudioClip;
                        m_audioSource.loop = true;
                        m_audioSource.Play();
                    }
                    break;
                case GhostAgent.State.Scatter:
                    goto case GhostAgent.State.Normal;
                case GhostAgent.State.Blue:
                    if (m_audioSource.clip != m_frightenAudioClip)
                    {
                        m_audioSource.clip = m_frightenAudioClip;
                        m_audioSource.loop = true;
                        m_audioSource.Play();
                    }
                    break;
                case GhostAgent.State.Dead:
                    if (m_audioSource.clip != m_deadGhostAudioClip)
                    {
                        m_audioSource.clip = m_deadGhostAudioClip;
                        m_audioSource.loop = true;
                        m_audioSource.Play();
                    }
                    break;
                case GhostAgent.State.Waiting:
                    break;
                default:
                    throw new ArgumentOutOfRangeException();
            }

            globalState = highestState;
        }
    }

    public void IncScore(int amount)
    {
        m_score += amount;
        m_dotCounter += 1;
        m_onScoreChanged.Invoke(m_score);

        if (m_score > m_highScore)
        {
            m_highScore = m_score;
            m_onHighScoreChanged.Invoke(m_highScore);
        }

        InkyAgent inky = FindObjectOfType<InkyAgent>();
        if (inky.IsWaiting && m_dotCounter >= m_inkyDotLimit)
        {
            inky.StopWaiting();
        }
        else
        {
            ClydeAgent clyde = FindObjectOfType<ClydeAgent>();
            if (clyde.IsWaiting && m_dotCounter >= m_clydeDotLimit)
            {
                clyde.StopWaiting();
            }
        }
    }

    public void FrightenGhosts()
    {
        GhostAgent[] ghosts = FindObjectsOfType<GhostAgent>();
        foreach (GhostAgent ghostAgent in ghosts)
        {
            ghostAgent.TurnBlue();
        }
    }

    public void PacmanDied()
    {
        AiAgent[] agents = FindObjectsOfType<AiAgent>();
        foreach (AiAgent aiAgent in agents)
        {
            aiAgent.Kill();
        }

        m_lives--;
    }

    public void ResetGame()
    {
        m_onLivesChanged.Invoke(m_lives);
    }

    private int m_score;

    private int m_highScore;

    private int m_lives = 3;

    private int m_dotCounter;

    [SerializeField] private ScoreChangedEvent m_onScoreChanged;

    [SerializeField] private ScoreChangedEvent m_onHighScoreChanged;

    [SerializeField] private ScoreChangedEvent m_onLivesChanged;

    [SerializeField] private AudioClip m_ghostAudioClip;

    [SerializeField] private AudioClip m_frightenAudioClip;

    [SerializeField] private AudioClip m_deadGhostAudioClip;

    [SerializeField] private AudioClip m_pacmanDeathClip;

    private AudioSource m_audioSource;

    private AgentInfo m_pacmanAgentInfo, m_blinkyAgentInfo, m_pinkyAgentInfo, m_inkyAgentInfo, m_clydeAgentInfo;

    private int m_inkyDotLimit = 30;
    private int m_clydeDotLimit = 90;

    private GhostAgent.State globalState = GhostAgent.State.Invalid;

    private static Game s_instance;
}
