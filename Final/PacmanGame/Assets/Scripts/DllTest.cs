using UnityEngine;
using System.Collections;
using System.Runtime.InteropServices;

[StructLayout(LayoutKind.Sequential)]
public struct TestStruct
{
    public float a;
    public float b;
}

public class DllTest : MonoBehaviour
{
	// Use this for initialization
	void Start ()
	{
	    TestStruct t = new TestStruct {a = 513};
	    Debug.Log(Test(0, 1, t));
	}

    [DllImport("ThinkerLib", CallingConvention = CallingConvention.Cdecl)]
    private static extern int Test(int a, int b, TestStruct testStruct);
}
