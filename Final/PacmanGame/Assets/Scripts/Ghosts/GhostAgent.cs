using System;
using UnityEngine;
using System.Collections;

public class GhostAgent : AiAgent
{
    public enum State
    {
        Normal,
        Blue,
        Dead,
        Waiting
    }

    public State CurrentState
    {
        get { return m_state; }
    }

    public void TurnBlue()
    {
        if (m_state != State.Dead)
        {
            m_state = State.Blue;
            m_animator.SetBool("IsBlue", true);
            m_animator.SetBool("IsBlinking", false);
            StopCoroutine("BlueTimer");
            StartCoroutine("BlueTimer");
        }
    }

    public void TurnNormal()
    {
        m_state = State.Normal;
        m_animator.SetBool("IsDead", false);
        m_animator.SetBool("IsBlue", false);
        m_animator.SetBool("IsBlinking", false);
        StopCoroutine("BlueTimer");
    }

    public void TurnDead()
    {
        if (m_state == State.Blue)
        {
            m_state = State.Dead;
            m_animator.SetBool("IsDead", true);
            m_animator.SetBool("IsBlue", false);
            m_animator.SetBool("IsBlinking", false);
            StopCoroutine("BlueTimer");
        }
    }

    protected override void Start()
    {
        m_animator = GetComponentInChildren<Animator>();
        base.Start();
    }

    void Update()
    {
        Vector2 movingDirection = Movement.MoveDirection;
        if (movingDirection != Vector2.zero)
        {
            m_animator.SetFloat("horizontal", movingDirection.x);
            m_animator.SetFloat("vertical", movingDirection.y);
        }
    }

    void OnTriggerEnter2D(Collider2D other)
    {
        if (other.tag == "Pacman")
        {
            switch (m_state)
            {
                case State.Normal:
                    break;
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
        m_agentInfo.State = (int)CurrentState;
    }

    public override bool CanTravelOnConnection(Connection connection)
    {
        return m_state != State.Waiting;
    }

    protected override Direction OnThink(float deltaTime, float totalTime)
    {
        throw new System.NotImplementedException();
    }

    protected override void OnThreadKill()
    {
        throw new System.NotImplementedException();
    }

    protected override void OnKill()
    {
        throw new System.NotImplementedException();
    }

    private IEnumerator BlueTimer()
    {
        yield return new WaitForSeconds(Game.Instance.FrightenTime - 2.0f);
        m_animator.SetBool("IsBlinking", true);
        yield return new WaitForSeconds(2.0f);
        TurnNormal();
    }

    private Animator m_animator;

    [SerializeField]
    private State m_state = State.Normal;
}
