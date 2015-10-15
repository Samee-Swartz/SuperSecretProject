

using System.Runtime.InteropServices;
using UnityEditor.AnimatedValues;
using UnityEngine;

public abstract class ThinkerEvent
{
    public abstract void Apply(AiAgent agent);
}

public class NodeCreateEvent : ThinkerEvent
{
    public NodeCreateEvent(Node node)
    {
        m_node = node;
        m_position = new NativeVector2(node.transform.position);
    }

    public override void Apply(AiAgent agent)
    {
        CreateNode(agent.WorldId, m_node.NodeId, m_position);
    }

    [DllImport("ThinkerLib", EntryPoint = "CreateNode", CallingConvention = CallingConvention.Cdecl)]
    private static extern int CreateNode(int worldId, int id, NativeVector2 position);

    private Node m_node;
    private NativeVector2 m_position;
}

public class ConnectionCreateEvent : ThinkerEvent
{
    public ConnectionCreateEvent(int from, int to, int direction, float cost)
    {
        m_from = from;
        m_to = to;
        m_cost = cost;
        m_direction = direction;
    }

    public override void Apply(AiAgent agent)
    {
        CreateConnection(agent.WorldId, m_from, m_to, m_direction, m_cost);
    }

    [DllImport("ThinkerLib", EntryPoint = "CreateConnection", CallingConvention = CallingConvention.Cdecl)]
    private static extern void CreateConnection(int worldId, int from, int to, int direction, float cost);

    private int m_from, m_to;
    private float m_cost;
    private int m_direction;
}

public class PointObjCreateEvent : ThinkerEvent
{
    public PointObjCreateEvent(int nodeId, int worth, int type)
    {
        m_nodeId = nodeId;
        m_worth = worth;
        m_type = type;
    }

    public override void Apply(AiAgent agent)
    {
        CreatePointObj(agent.WorldId, m_nodeId, m_worth, m_type);
    }

    [DllImport("ThinkerLib", EntryPoint = "CreatePointObj", CallingConvention = CallingConvention.Cdecl)]
    private static extern void CreatePointObj(int worldId, int nodeId, int worth, int type);

    private int m_nodeId;

    private int m_worth;

    private int m_type;
}

public class PointObjDestroyEvent : ThinkerEvent
{
    public PointObjDestroyEvent(int nodeId)
    {
        m_nodeId = nodeId;
    }

    public override void Apply(AiAgent agent)
    {
        DestroyPointObj(agent.WorldId, m_nodeId);
    }

    [DllImport("ThinkerLib", EntryPoint = "DestroyPointObj", CallingConvention = CallingConvention.Cdecl)]
    private static extern void DestroyPointObj(int worldId, int nodeId);

    private int m_nodeId;
}