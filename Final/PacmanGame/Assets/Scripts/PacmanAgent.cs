using UnityEngine;
using System.Collections;
using System.Runtime.InteropServices;

public class PacmanAgent : AiAgent
{
    public override bool CanTravelOnConnection(Connection connection)
    {
        return connection.TargetNode.tag != "NoPacmanNode";
    }

    protected override void OnStartAgent(bool isInit)
    {
        m_animator = GetComponentInChildren<Animator>();
        m_animator.speed = 1.0f;
        m_isDead = false;
        m_animator.SetBool("IsDead", false);

        if (!isInit)
            Movement.enabled = true;

        base.OnStartAgent(isInit);
    }

    void Update()
    {
        if (m_isDead)
            return;

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
        return (Direction) NativeThink3(WorldId, deltaTime, totalTime);
    }

    protected override void OnThreadKill()
    {
    }

    protected override void OnKill()
    {
        m_animator.speed = 1;
        m_animator.SetBool("IsDead", true);
        m_isDead = true;
    }

    [DllImport("ThinkerLib", EntryPoint = "ThinkPacman", CallingConvention = CallingConvention.Cdecl)]
    private static extern int NativeThink(int worldId, float deltaTime, float totalTime);

    [DllImport("ThinkerLib", EntryPoint = "ThinkPacman2", CallingConvention = CallingConvention.Cdecl)]
    private static extern int NativeThink2(int worldId, float deltaTime, float totalTime);

    [DllImport("ThinkerLib", EntryPoint = "ThinkPacman3", CallingConvention = CallingConvention.Cdecl)]
    private static extern int NativeThink3(int worldId, float deltaTime, float totalTime);

    private Animator m_animator;

    private bool m_isDead;
}
