using UnityEngine;
using System.Collections;
using System.Runtime.InteropServices;

public class PacmanAgent : AiAgent
{
    public override bool CanTravelOnConnection(Connection connection)
    {
        return connection.TargetNode.tag != "NoPacmanNode";
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
            float angle = Mathf.Atan2(movingDirection.y, movingDirection.x) * Mathf.Rad2Deg;
            transform.rotation = Quaternion.Euler(0, 0, angle);
            m_animator.speed = 1;
        }
        else
        {
            m_animator.speed = 0;
        }
    }

    protected override void FixedUpdate()
    {
        base.FixedUpdate();
    }

    protected override Direction OnThink(float deltaTime, float totalTime)
    {
        return Movement.InputDirection;
        return (Direction) ThinkPacman(WorldId, deltaTime, totalTime);
    }

    protected override void OnThreadKill()
    {
    }

    protected override void OnKill()
    {
        m_animator.SetBool("IsDead", true);
    }

    [DllImport("ThinkerLib", CallingConvention = CallingConvention.Cdecl)]
    private static extern int ThinkPacman(int worldId, float deltaTime, float totalTime);

    private Animator m_animator;
}
