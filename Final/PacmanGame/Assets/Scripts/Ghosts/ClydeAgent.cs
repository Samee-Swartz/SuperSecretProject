using UnityEngine;
using System.Collections;
using System.Runtime.InteropServices;

public class ClydeAgent : GhostAgent
{
    protected override Direction OnThink(float deltaTime, float totalTime)
    {
        return (Direction) NativeThink(WorldId, deltaTime, totalTime);
    }

    [DllImport("ThinkerLib", EntryPoint = "ThinkClyde", CallingConvention = CallingConvention.Cdecl)]
    private static extern int NativeThink(int worldId, float deltaTime, float totalTime);
}
