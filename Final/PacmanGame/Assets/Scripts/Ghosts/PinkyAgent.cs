using UnityEngine;
using System.Collections;
using System.Runtime.InteropServices;

public class PinkyAgent : GhostAgent
{
    protected override void OnStartAgent(bool isInit)
    {
        base.OnStartAgent(isInit);
        Movement.InputDirection = Direction.Up;
    }

    protected override Direction OnThink(float deltaTime, float totalTime)
    {
        return (Direction) NativeThink(WorldId, deltaTime, totalTime);
    }

    [DllImport("ThinkerLib", EntryPoint = "ThinkPinky", CallingConvention = CallingConvention.Cdecl)]
    private static extern int NativeThink(int worldId, float deltaTime, float totalTime);
}
