using System;
using UnityEngine;
using System.Collections;
using UnityEngine.Events;

[Serializable]
public class ScoreChangedEvent : UnityEvent<int>
{
    
}

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
    }

    void FixedUpdate()
    {
        lock (this)
        {
            m_pacmanAgentInfo = FindObjectOfType<PacmanAgent>().Info;
            //m_blinkyAgentInfo = FindObjectOfType<BlinkyAgent>().Info;
            //m_pinkyAgentInfo = FindObjectOfType<PinkyAgent>().Info;
            //m_inkyAgentInfo = FindObjectOfType<InkyAgent>().Info;
            //m_clydeAgentInfo = FindObjectOfType<ClydeAgent>().Info;
        }
    }

    public void IncScore(int amount)
    {
        m_score += amount;
        m_onScoreChanged.Invoke(m_score);
    }

    public void DecScore(int amount)
    {
        m_score -= amount;
        m_onScoreChanged.Invoke(m_score);
    }

    public void FrightenGhosts()
    {
        GhostAgent[] ghosts = FindObjectsOfType<GhostAgent>();
        foreach (GhostAgent ghostAgent in ghosts)
        {
            ghostAgent.TurnBlue();
        }
    }

    private int m_score;

    [SerializeField]
    private ScoreChangedEvent m_onScoreChanged;

    private AgentInfo m_pacmanAgentInfo, m_blinkyAgentInfo, m_pinkyAgentInfo, m_inkyAgentInfo, m_clydeAgentInfo;

    private static Game s_instance;
}
