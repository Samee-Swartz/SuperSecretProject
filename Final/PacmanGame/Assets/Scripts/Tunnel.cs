using UnityEngine;
using System.Collections;

public class Tunnel : MonoBehaviour
{
    void OnTriggerEnter2D(Collider2D other)
    {
        MovingAgent agent = other.GetComponentInParent<MovingAgent>();
        if (agent != null)
            agent.TeleportTo(m_destinationNode);
    }

    [SerializeField]
    private Node m_destinationNode;
}
