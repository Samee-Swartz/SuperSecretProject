using System;
using UnityEngine;
using System.Collections;

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
            m_state = State.Normal;
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

    protected override void Start()
    {
        m_animator = GetComponentInChildren<Animator>();
        base.Start();
        Movement.enabled = m_state != State.Waiting;
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
                    Game.Instance.PacmanDied();
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
        m_state = Movement.enabled ? State.Normal : State.Waiting;
        m_animator.SetBool("IsDead", false);
        m_animator.SetBool("IsBlue", false);
        m_animator.SetBool("IsBlinking", false);
        StopCoroutine("BlueTimer");
    }

    private Animator m_animator;

    [SerializeField]
    private State m_state = State.Normal;

    private static int s_eatenGhosts;
}
