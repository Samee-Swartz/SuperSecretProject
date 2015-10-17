using System;
using UnityEngine;
using System.Collections;
using UnityEditor.iOS.Xcode;

public class GhostAgent : AiAgent
{
    public enum State
    {
        Invalid = -1,
        Normal,
        Scatter,
        Blue,
        Dead,
        Waiting
    }

    public State CurrentState
    {
        get { return m_state; }
    }

    public override bool IsWaiting
    {
        get { return m_state == State.Waiting || !Movement.enabled; }
    }

    public void TurnBlue()
    {
        if (m_state != State.Dead)
        {
            s_eatenGhosts = 0;
            m_state = State.Blue;
            m_animator.SetBool("IsBlue", true);
            m_animator.SetBool("IsBlinking", false);
            StopCoroutine("BlueTimer");
            StartCoroutine("BlueTimer");
        }
    }

    public override void StopWaiting()
    {
        if (!Movement.enabled)
            Movement.enabled = true;

        if(m_state == State.Waiting)
            m_state = Game.Instance.IsScatter ? State.Scatter : State.Normal;
    }

    public void TurnDead()
    {
        if (m_state == State.Blue)
        {
            s_eatenGhosts += 1;
            Game.Instance.IncScore((int) Mathf.Pow(200.0f, s_eatenGhosts));

            m_state = State.Dead;
            m_animator.SetBool("IsDead", true);
            m_animator.SetBool("IsBlue", false);
            m_animator.SetBool("IsBlinking", false);
            StopCoroutine("BlueTimer");
        }
    }

    protected override void OnStartAgent(bool isInit)
    {
        if (isInit)
            m_originalState = m_state;

        m_animator = GetComponentInChildren<Animator>();
        base.OnStartAgent(isInit);
        Movement.enabled = m_originalState != State.Waiting;
    }

    void Update()
    {
        Vector2 movingDirection = Movement.MoveDirection;
        if (movingDirection != Vector2.zero)
        {
            m_animator.SetFloat("horizontal", movingDirection.x);
            m_animator.SetFloat("vertical", movingDirection.y);
        }

        if (m_state == State.Normal && Game.Instance.IsScatter)
        {
            m_state = State.Scatter;
        }
        else if (m_state == State.Scatter && !Game.Instance.IsScatter)
        {
            m_state = State.Normal;
        }
        else if (m_state == State.Dead && Movement.AtNode && Movement.AtNode.tag == "NoPacmanNode")
        {
            TurnNormalInternal();
        }

        switch (m_state)
        {
            case State.Invalid:
                break;
            case State.Normal:
                Movement.MaxSpeed = 1.1f;
                break;
            case State.Scatter:
                goto case State.Normal;
            case State.Blue:
                Movement.MaxSpeed = 0.6f;
                break;
            case State.Dead:
                Movement.MaxSpeed = 2.1f;
                break;
            case State.Waiting:
                break;
            default:
                throw new ArgumentOutOfRangeException();
        }
    }

    private void OnTriggerEnter2D(Collider2D other)
    {
        if (other.tag == "Pacman")
        {
            switch (m_state)
            {
                case State.Normal:
                    Game.Instance.PacmanDied();
                    break;
                case State.Scatter:
                    goto case State.Normal;
                case State.Blue:
                    TurnDead();
                    break;
                case State.Dead:
                    break;
                case State.Waiting:
                    break;
                default:
                    throw new ArgumentOutOfRangeException();
            }
        }
    }

    protected override void FixedUpdate()
    {
        base.FixedUpdate();
        m_agentInfo.State = (int) CurrentState;
    }

    public override bool CanTravelOnConnection(Connection connection)
    {
        return m_state != State.Waiting;
    }

    protected override Direction OnThink(float deltaTime, float totalTime)
    {
        return Direction.Up;
    }

    protected override void OnThreadKill()
    {
    }

    protected override void OnKill()
    {
    }

    private IEnumerator BlueTimer()
    {
        yield return new WaitForSeconds(Game.Instance.FrightenTime - 2.0f);
        m_animator.SetBool("IsBlinking", true);
        yield return new WaitForSeconds(2.0f);
        TurnNormalInternal();
    }

    private void TurnNormalInternal()
    {
        if (IsWaiting)
        {
            m_state = State.Waiting;
        }
        else
        {
            if (Game.Instance.IsScatter)
                m_state = State.Scatter;
            else
                m_state = State.Normal;
        }
        m_animator.SetBool("IsDead", false);
        m_animator.SetBool("IsBlue", false);
        m_animator.SetBool("IsBlinking", false);
        StopCoroutine("BlueTimer");
    }

    private Animator m_animator;

    [SerializeField] private State m_state = State.Normal;

    private State m_originalState;

    private static int s_eatenGhosts;
}
