using UnityEngine;
using System.Collections;
using System.Runtime.InteropServices;

public class InkyAgent : GhostAgent
{
    protected override Direction OnThink(float deltaTime, float totalTime)
    {
        return (Direction) NativeThink(WorldId, deltaTime, totalTime);
    }

    [DllImport("ThinkerLib", EntryPoint = "ThinkInky", CallingConvention = CallingConvention.Cdecl)]
    private static extern int NativeThink(int worldId, float deltaTime, float totalTime);
}
