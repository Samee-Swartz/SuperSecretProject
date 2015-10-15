using UnityEngine;
using UnityEditor;
using System.Collections;

public class ConvertTo2D
{
    [MenuItem("GameObject/ConvertBoxCollidersTo2D")]
    public static void ConvertSelectedBoxColliders()
    {
        GameObject[] objs = Selection.gameObjects;
        foreach (GameObject gameObject in objs)
        {
            BoxCollider boxCollider = gameObject.GetComponent<BoxCollider>();
            if (boxCollider == null)
                continue;

            Vector2 center = boxCollider.center;
            Vector2 size = boxCollider.size;

            Object.DestroyImmediate(boxCollider);

            BoxCollider2D newCollider = gameObject.AddComponent<BoxCollider2D>();
            newCollider.offset = center;
            newCollider.size = size;
        }
    }
}
