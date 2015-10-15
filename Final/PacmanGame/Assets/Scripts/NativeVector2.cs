using UnityEngine;
using System.Collections;
using System.Runtime.InteropServices;

[StructLayout(LayoutKind.Sequential)]
public struct NativeVector2
{
    public NativeVector2(float x, float y)
    {
        m_x = x;
        m_y = y;
    }

    public NativeVector2(Vector2 vector)
    {
        m_x = vector.x;
        m_y = vector.y;
    }

    public static implicit operator Vector2(NativeVector2 nativeVector)
    {
        return new Vector2(nativeVector.m_x, nativeVector.m_y);
    }

    public float m_x;
    public float m_y;
}
