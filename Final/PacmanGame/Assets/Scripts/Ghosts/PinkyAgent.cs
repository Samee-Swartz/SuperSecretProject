using UnityEngine;
using System.Collections;

public class PinkyAgent : GhostAgent
{
    protected override void Start()
    {
        base.Start();
        Movement.InputDirection = Direction.Up;
    }
}
