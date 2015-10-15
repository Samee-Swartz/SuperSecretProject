using UnityEngine;
using System.Collections;

public class ScoreNode : Node
{
    public enum Type
    {
        Dot,
        BigDot,
        Fruit
    }

    public int Score
    {
        get { return m_score; }
    }

    public void Activate()
    {
        transform.GetChild(0).gameObject.SetActive(true);

        AiAgent[] agents = FindObjectsOfType<AiAgent>();
        foreach (AiAgent aiAgent in agents)
        {
            aiAgent.AddEvent(new PointObjCreateEvent(NodeId, m_score, (int)m_type));
        }
    }

    void OnTriggerEnter2D(Collider2D otherCollider2D)
    {
        if (otherCollider2D.tag == "Pacman")
        {
            transform.GetChild(0).gameObject.SetActive(false);
            Game.Instance.IncScore(m_score);

            AiAgent[] agents = FindObjectsOfType<AiAgent>();
            foreach (AiAgent aiAgent in agents)
            {
                aiAgent.AddEvent(new PointObjDestroyEvent(NodeId));
            }

            OnPickup();

            if(m_clip != null)
                AudioSource.PlayClipAtPoint(m_clip, Vector3.zero);
        }
    }

    protected override void Start()
    {
        base.Start();
        if (m_enabledOnStart)
            Activate();
        else
            transform.GetChild(0).gameObject.SetActive(false);
    }

    protected virtual void OnPickup()
    {
        
    }

#if UNITY_EDITOR
    protected override void OnDrawGizmos()
    {
    }
#endif

    [SerializeField]
    private int m_score = 10;

    [SerializeField]
    private Type m_type = Type.Dot;

    [SerializeField, Tooltip("The sound to play when this object is picked up.")]
    private AudioClip m_clip;

    [SerializeField]
    private bool m_enabledOnStart = true;
}
