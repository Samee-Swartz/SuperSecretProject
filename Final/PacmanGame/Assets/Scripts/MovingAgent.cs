using System;
using UnityEngine;
using System.Collections;
using UnityEditor.AnimatedValues;
using UnityEngine.Assertions;

[RequireComponent(typeof(Rigidbody2D))]
public class MovingAgent : MonoBehaviour
{
    public Direction InputDirection
    {
        get { return m_inputDirection; }
        set { m_inputDirection = value; }
    }

    public Vector2 MoveDirection
    {
        get { return m_moveDirection; }
    }

    public float Speed
    {
        get { return m_moveDirection.magnitude * m_speed; }
    }

    public Node ClosestNode
    {
        get { return m_closestNode; }
    }

    public Node AtNode
    {
        get { return m_atNode; }
    }

    public Node[] ClosestNodes
    {
        get { return (Node[])m_closestNodes.Clone(); }
    }

    public void TeleportTo(Node destinationNode)
    {
        m_rigidbody2D.position = destinationNode.transform.position;
        m_atNode = destinationNode;
        m_closestNode = destinationNode;
        CalculateClosestNodes();
    }

    void Start()
    {
        m_rigidbody2D = GetComponent<Rigidbody2D>();

        Node[] nodes = FindObjectsOfType<Node>();

        Vector3 position = transform.position;

        float bestDistance = float.MaxValue;
        foreach (Node node in nodes)
        {
            
            float sqrMagnitude = (node.transform.position - position).sqrMagnitude;
            if (sqrMagnitude < bestDistance)
            {
                m_closestNode = node;
                bestDistance = sqrMagnitude;
            }
        }
    }

    void OnDisable()
    {
        m_rigidbody2D.velocity = Vector2.zero;
    }

    protected virtual void Update()
    {
        CalculateClosestNodes();
        ProcessInput();
        DoMove();
    }

    private bool CanTravelOnConnection(Connection connection)
    {
        return GetComponent<AiAgent>().CanTravelOnConnection(connection);
    }

    private void CalculateClosestNodes()
    {
        float distanceSqrToClosestNode = (m_closestNode.transform.position - transform.position).sqrMagnitude;
        int connectionCount = m_closestNode.GetConnectionCount();

        float closestDistanceSqr = distanceSqrToClosestNode;
        int closestIndex = -1;
        Node closestNode = null;
        for (int i = 0; i < connectionCount; i++)
        {
            Connection connection = m_closestNode.GetConnection(i);
            if(!Connection.IsValid(connection))
                continue;

            Node node = connection.TargetNode;
            var sqrDistance = (transform.position - node.transform.position).sqrMagnitude;
            if (sqrDistance < closestDistanceSqr)
            {
                closestIndex = i;
                closestDistanceSqr = sqrDistance;
                closestNode = node;
            }
        }

        //Find the new closest node if we have one
        if (closestIndex != -1)
            m_closestNode = closestNode;

        //check to see if we are at the closest node
        float distanceToClosest = (m_closestNode.transform.position - transform.position).magnitude;
        m_atNode = distanceToClosest < m_atNodeDistance ? m_closestNode : null;

        if (m_atNode == null)
        {
            Vector2 vectorFromNode = transform.position - m_closestNode.transform.position;
            Direction directionFromNode = DirectionHelper.VectorToDirection(vectorFromNode);
            Direction oppositeDirection = DirectionHelper.OppositeDirection(directionFromNode);

            for (int i = 0; i < m_closestNodes.Length; i++)
            {
                m_closestNodes[i] = null;
            }

            m_closestNodes[(int) oppositeDirection] = m_closestNode;

            Connection otherDirectionConnection = m_closestNode.GetConnection(directionFromNode);
            if (Connection.IsValid(otherDirectionConnection))
                m_closestNodes[(int) directionFromNode] = otherDirectionConnection.TargetNode;
        }
        else
        {
            connectionCount = m_atNode.GetConnectionCount();
            for (int i = 0; i < connectionCount; i++)
            {
                Connection connection = m_atNode.GetConnection(i);
                m_closestNodes[i] = Connection.IsValid(connection) ? connection.TargetNode : null;
            }
        }
    }

    private void ProcessInput()
    {
        if (m_atNode)
        {
            Connection inputConnection = m_atNode.GetConnection(m_inputDirection);
            if (!Connection.IsValid(inputConnection) || !CanTravelOnConnection(inputConnection))
            {
                Direction continueDirection = DirectionHelper.VectorToDirection(m_moveDirection);
                Connection continueConnection = m_atNode.GetConnection(continueDirection);

                if(!Connection.IsValid(continueConnection) || !CanTravelOnConnection(continueConnection))
                    m_moveDirection = Vector2.zero;
            }
            else
                m_moveDirection = DirectionHelper.DirectionToVector(m_inputDirection);
        }
        else
        {
            Vector2 vectorFromNode = (transform.position - m_closestNode.transform.position);
            Direction directionFromNode = DirectionHelper.VectorToDirection(vectorFromNode);

            Assert.AreNotEqual(Direction.Invalid, directionFromNode);

            Direction directionToNode = DirectionHelper.OppositeDirection(directionFromNode);

            if (m_inputDirection == directionFromNode)
                m_moveDirection = DirectionHelper.DirectionToVector(directionFromNode);
            else if (m_inputDirection == directionToNode)
                m_moveDirection = DirectionHelper.DirectionToVector(directionToNode);
        }
    }

    private void DoMove()
    {
        m_rigidbody2D.velocity = m_moveDirection * m_speed;

        if (m_closestNode)
        {
            if (m_moveDirection == Vector2.zero)
                m_rigidbody2D.position = m_closestNode.transform.position;
            else if (Math.Abs(Vector2.Dot(m_moveDirection.normalized, Vector2.right)) > 0.9f)
                m_rigidbody2D.position = new Vector2(m_rigidbody2D.position.x, m_closestNode.transform.position.y);
            else
                m_rigidbody2D.position = new Vector2(m_closestNode.transform.position.x, m_rigidbody2D.position.y);
        }
    }

    [SerializeField]
    private Node m_closestNode;

    [SerializeField]
    private Node m_atNode;

    [SerializeField]
    private Node[] m_closestNodes = new Node[4];

    private Direction m_inputDirection = Direction.Right;

    private Vector2 m_moveDirection = Vector2.right;

    private Rigidbody2D m_rigidbody2D;

    [SerializeField]
    private float m_atNodeDistance = 0.1f;

    [SerializeField]
    private float m_speed = 0.1f;
}
