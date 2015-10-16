using UnityEngine;
using System.Collections;
using System.Runtime.InteropServices;

public class BlinkyAgent : GhostAgent
{
    protected override Direction OnThink(float deltaTime, float totalTime)
    {
        return (Direction) BlinkyNativeThink(WorldId, deltaTime, totalTime);
    }

    [DllImport("ThinkerLib", EntryPoint = "ThinkBlinky", CallingConvention = CallingConvention.Cdecl)]
    private static extern int BlinkyNativeThink(int worldId, float deltaTime, float totalTime);
}
