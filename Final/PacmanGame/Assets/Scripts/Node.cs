using System;
using UnityEngine;
using System.Collections.Generic;
#if UNITY_EDITOR
using UnityEditor;
#endif
using Random = UnityEngine.Random;

[Serializable]
public class Connection
{
    public static bool IsValid(Connection connection)
    {
        return connection != null && connection.TargetNode != null;
    }

    public Node TargetNode
    {
        get { return m_node; }
        set { m_node = value; }
    }

    public float Cost
    {
        get { return m_cost; }
        set { m_cost = value; }
    }

    public bool IsStatic
    {
        get { return m_isStatic; }
    }

    public Connection(Node target, float cost)
    {
        m_node = target;
        m_cost = cost;
    }

    [SerializeField]
    private Node m_node;

    [SerializeField]
    private float m_cost;

    [SerializeField, Tooltip("If true then this connection will not be rebuilt when the navigation is rebuilt")]
    private bool m_isStatic;
}


public class Node : MonoBehaviour
{
    public int NodeId
    {
        get { return m_nodeId; }
    }

    public Connection GetConnection(Direction connectionDirection)
    {
        int connectionIndex = (int)connectionDirection;
        if (connectionIndex < 0 || connectionIndex >= m_connections.Length)
            return null;
        return m_connections[connectionIndex];
    }

    public Connection GetConnection(int connectionId)
    {
        return m_connections[connectionId];
    }

    public int GetConnectionCount()
    {
        return m_connections.Length;
    }

    protected virtual void Start()
    {
        AiAgent[] agents = FindObjectsOfType<AiAgent>();
        foreach (AiAgent aiAgent in agents)
        {
            aiAgent.AddEvent(new NodeCreateEvent(this));

            for (int i = 0; i < m_connections.Length; i++)
            {
                Connection connection = GetConnection(i);
                if(Connection.IsValid(connection))
                    aiAgent.AddEvent(new ConnectionCreateEvent(NodeId, connection.TargetNode.NodeId, i, connection.Cost));
            }
        }
    }

#if UNITY_EDITOR

    [MenuItem("Navigation/Rebuild")]
    public static void RebuildNodes()
    {
        Node[] nodes = FindObjectsOfType<Node>();
        foreach (Node node in nodes)
        {
            node.ReBuild();
        }
    }

    public virtual void ReBuild()
    {
        if (m_nodeId <= 0)
            m_nodeId = Random.Range(0, int.MaxValue);

            Node[] otherNodes = FindObjectsOfType<Node>();
        List<Node> nodeList = new List<Node>(otherNodes);
        nodeList.Remove(this);
        nodeList.Sort((a, b) =>
        {
            float aCost = (a.transform.position - transform.position).magnitude;
            float bCost = (b.transform.position - transform.position).magnitude;

            if (aCost > bCost)
                return 1;
            else if(aCost < bCost)
                return -1;
            else
                return 0;
        });

        if (m_connections == null || m_connections.Length < 4)
        {
            m_connections = new Connection[4];
        }
        else
        {
            for (int i = 0; i < m_connections.Length; i++)
            {
                if(m_connections[i] != null && !m_connections[i].IsStatic)
                    m_connections[i] = null;
            }
        }

        int directionFillCount = 0;
        foreach (Node node in nodeList)
        {
            Vector2 offset = (node.transform.position - transform.position);
            Vector2 direction = offset.normalized;
            float cost = offset.magnitude;
            if (cost > m_maxBuildCost)
                break;

            if (m_useCostOverride)
            {
                if (node.m_useCostOverride)
                    cost = m_fromCostOverride + node.m_toCostOverride;
                else
                    cost = m_fromCostOverride;
            }
            else if (node.m_useCostOverride)
            {
                cost = node.m_toCostOverride;
            }

            if (Vector2.Dot(direction, Vector2.up) > 0.99f)
            {
                if(m_connections[0] == null)
                {
                    directionFillCount++;
                    m_connections[0] = new Connection(node, cost);
                }
            }
            else if (Vector2.Dot(direction, Vector2.right) > 0.99f)
            {
                if (m_connections[1] == null)
                {
                    directionFillCount++;
                    m_connections[1] = new Connection(node, cost);
                }
            }
            else if (Vector2.Dot(direction, Vector2.down) > 0.99f)
            {
                if (m_connections[2] == null)
                {
                    directionFillCount++;
                    m_connections[2] = new Connection(node, cost);
                }
            }
            else if (Vector2.Dot(direction, Vector2.left) > 0.99f)
            {
                if (m_connections[3] == null)
                {
                    directionFillCount++;
                    m_connections[3] = new Connection(node, cost);
                }
            }

            if (directionFillCount >= 4)
                break;
        }

        for (int i = 0; i < m_connections.Length; i++)
        {
            Connection connection = m_connections[i];
            if (connection == null || connection.IsStatic)
                continue;

            RaycastHit2D hit = Physics2D.Linecast(connection.TargetNode.transform.position, transform.position, LayerMask.GetMask("Wall"));
            if (hit.collider != null)
                m_connections[i] = null;
        }
    }

    protected virtual void OnDrawGizmos()
    {
        Gizmos.color = Color.white;
        Gizmos.DrawSphere(transform.position, 0.1f);
    }

    protected virtual void OnDrawGizmosSelected()
    {
        Gizmos.color = Color.white;

        foreach (Connection connection in m_connections)
        {
            if(connection != null && connection.TargetNode != null)
            {
                Vector3 point1 = transform.position;
                Vector3 point2 = connection.TargetNode.transform.position;
                Vector3 direction = (point1 - point2).normalized;

                point1 += (-direction)*0.1f;
                point2 += direction*0.1f;

                Gizmos.DrawLine(point1, point2);
            }
        }
    }
#endif

    [SerializeField]
    private int m_nodeId = -1;

    [SerializeField]
    private Connection[] m_connections;

    [SerializeField]
    private float m_maxBuildCost = 0.3f;

    [SerializeField]
    private bool m_useCostOverride = false;

    [SerializeField]
    private float m_toCostOverride;

    [SerializeField]
    private float m_fromCostOverride;

    private static bool needsRebuild = false;
}
