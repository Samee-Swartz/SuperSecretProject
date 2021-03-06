﻿using System;
using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Threading;

[StructLayout(LayoutKind.Sequential)]
public struct AgentInfo
{
    public struct NextNodesContainer
    {
        public int this[int i]
        {
            get
            {
                Direction direction = (Direction) i;
                switch (direction)
                {
                    case Direction.Invalid:
                        throw new InvalidOperationException();
                    case Direction.Up:
                        return UpNode;
                    case Direction.Right:
                        return RightNode;
                    case Direction.Down:
                        return DownNode;
                    case Direction.Left:
                        return LeftNode;
                    default:
                        throw new ArgumentOutOfRangeException();
                }
            }
            set
            {
                Direction direction = (Direction)i;
                switch (direction)
                {
                    case Direction.Invalid:
                        throw new InvalidOperationException();
                    case Direction.Up:
                        UpNode = value;
                        break;
                    case Direction.Right:
                        RightNode = value;
                        break;
                    case Direction.Down:
                        DownNode = value;
                        break;
                    case Direction.Left:
                        LeftNode = value;
                        break;
                    default:
                        throw new ArgumentOutOfRangeException();
                }
            }
        }

        public int UpNode;
        public int RightNode;
        public int DownNode;
        public int LeftNode;
    }

    public Vector2 Position;
    public int AtNode;
    public int ClosestNode;
    public NextNodesContainer NextNodes;
    public float Speed;
    public int FacingDirection;
    public int State;
}

[RequireComponent(typeof (MovingAgent))]
public abstract class AiAgent : MonoBehaviour
{
    public int WorldId
    {
        get { return m_worldId; }
    }

    public AgentInfo Info
    {
        get { return m_agentInfo; }
    }

    public virtual bool IsWaiting
    {
        get { return !m_movement.enabled; }
    }

    protected MovingAgent Movement
    {
        get { return m_movement; }
    }

    public bool HasInit
    {
        get { return m_hasInit; }
    }

    public virtual void Wait()
    {
        m_movement.enabled = false;
    }

    public virtual void StopWaiting()
    {
        m_movement.enabled = true;
    }

    public virtual bool CanTravelOnConnection(Connection connection)
    {
        return true;
    }

    public void AddEvent(ThinkerEvent thinkerEvent)
    {
        lock (this)
        {
            m_events.Add(thinkerEvent);
        }
    }

    public void StartAgent()
    {
        OnStartAgent(!m_hasInit);
        m_hasInit = true;
        StartCoroutine(StartThinkerThread());
    }

    private IEnumerator StartThinkerThread()
    {
        yield return new WaitForFixedUpdate();
        yield return null;

        m_thread = new Thread(Thinker);
        m_thread.Start();
    }

    protected virtual void OnStartAgent(bool isInit)
    {
        m_movement = GetComponent<MovingAgent>();
        m_isWaiting = m_movement.enabled;
        m_gameInstance = Game.Instance;

        if (isInit)
            m_startingNode = m_movement.AtNode ? m_movement.AtNode : m_movement.ClosestNode;
        else
            m_movement.TeleportTo(m_startingNode);
    }

    public void Kill()
    {
        m_thread.Interrupt();
        Wait();
        OnKill();
    }

    public void GetherAgentInfo()
    {
        lock (this)
        {
            if (m_direction != Direction.Invalid)
                m_movement.InputDirection = m_direction;

            m_agentInfo.Position = m_movement.transform.position;
            m_agentInfo.AtNode = m_movement.AtNode != null ? m_movement.AtNode.NodeId : -1;
            m_agentInfo.ClosestNode = m_movement.ClosestNode != null ? m_movement.ClosestNode.NodeId : -1;

            Node[] closestNodes = m_movement.ClosestNodes;
            for (int i = 0; i < closestNodes.Length; i++)
            {
                Node node = closestNodes[i];
                if (node != null)
                    m_agentInfo.NextNodes[i] = node.NodeId;
                else
                    m_agentInfo.NextNodes[i] = -1;
            }

            m_agentInfo.Speed = m_movement.Speed;

            if (m_movement.MoveDirection != Vector2.zero)
                m_agentInfo.FacingDirection = (int)DirectionHelper.VectorToDirection(m_movement.MoveDirection);

            m_isWaiting = !Movement.enabled || m_agentInfo.ClosestNode == -1;
        }
    }

    protected virtual void Start()
    {
        
    }

    protected virtual void FixedUpdate()
    {
        GetherAgentInfo();
    }

    private void OnDestroy()
    {
        Kill();
    }

    protected abstract Direction OnThink(float deltaTime, float totalTime);

    protected abstract void OnThreadKill();

    protected abstract void OnKill();

    private void SetMovingDirection(Direction newDirection)
    {
        lock (this)
        {
            m_direction = newDirection;
        }
    }

    [DllImport("ThinkerLib", EntryPoint = "CreateWorld", CallingConvention = CallingConvention.Cdecl)]
    private static extern int CreateNativeWorld();

    [DllImport("ThinkerLib", EntryPoint = "DestroyWorld", CallingConvention = CallingConvention.Cdecl)]
    private static extern void DestroyNativeWorld(int worldId);

    [DllImport("ThinkerLib", EntryPoint = "SetPacman", CallingConvention = CallingConvention.Cdecl)]
    private static extern void SetPacman(int worldId, ref AgentInfo agentInfo);

    [DllImport("ThinkerLib", EntryPoint = "SetBlinky", CallingConvention = CallingConvention.Cdecl)]
    private static extern void SetBlinky(int worldId, ref AgentInfo agentInfo);

    [DllImport("ThinkerLib", EntryPoint = "SetPinky", CallingConvention = CallingConvention.Cdecl)]
    private static extern void SetPinky(int worldId, ref AgentInfo agentInfo);

    [DllImport("ThinkerLib", EntryPoint = "SetInky", CallingConvention = CallingConvention.Cdecl)]
    private static extern void SetInky(int worldId, ref AgentInfo agentInfo);

    [DllImport("ThinkerLib", EntryPoint = "SetClyde", CallingConvention = CallingConvention.Cdecl)]
    private static extern void SetClyde(int worldId, ref AgentInfo agentInfo);

    private void Thinker()
    {
        DateTime startTime = DateTime.Now;
        DateTime lastTime = startTime;

        Thread.Sleep(100);

        if (m_worldId < 0)
            m_worldId = CreateNativeWorld();

        int eventIndex = 0;

        while (true)
        {
            AgentInfo pacman, inky, pinky, blinky, clyde;

            lock (this)
            {
                for (; eventIndex < m_events.Count; eventIndex++)
                {
                    m_events[eventIndex].Apply(this);
                }
            }

            lock (Game.Instance)
            {
                pacman = Game.Instance.PacmanAgentInfo;
                inky = Game.Instance.InkyAgentInfo;
                pinky = Game.Instance.PinkyAgentInfo;
                blinky = Game.Instance.BlinkyAgentInfo;
                clyde = Game.Instance.ClydeAgentInfo;
            }

            SetPacman(m_worldId, ref pacman);
            SetBlinky(m_worldId, ref blinky);
            SetPinky(m_worldId, ref pinky);
            SetInky(m_worldId, ref inky);
            SetClyde(m_worldId, ref clyde);

            DateTime now = DateTime.Now;
            TimeSpan deltaTime = now - lastTime;
            TimeSpan totalTime = now - startTime;

            lastTime = now;

            if(m_isWaiting)
                SetMovingDirection(Direction.Invalid);
            else
                SetMovingDirection(OnThink((float) deltaTime.TotalSeconds, (float) totalTime.TotalSeconds));

            TimeSpan thinkTime = DateTime.Now - lastTime;

            try
            {
                int sleepTime = Mathf.Max(1, 40 - (int) thinkTime.TotalMilliseconds);
                Thread.Sleep(sleepTime);
            }
            catch (ThreadInterruptedException)
            {
                OnThreadKill();
                DestroyNativeWorld(m_worldId);
                m_worldId = -1;
                return;
            }
        }
    }

    private Direction m_direction;

    private MovingAgent m_movement;

    private Thread m_thread;

    protected AgentInfo m_agentInfo;

    private int m_worldId = -1;

    private bool m_isWaiting = true;

    private bool m_hasInit = false;

    private Node m_startingNode;

    private List<ThinkerEvent> m_events = new List<ThinkerEvent>();

    private Game m_gameInstance;
}
