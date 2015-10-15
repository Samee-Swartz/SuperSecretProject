using UnityEngine;
using System.Collections;

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

    private Animator m_animator;
}
