using UnityEngine;
using System.Collections;

public class ChildCollider : MonoBehaviour
{
    void OnTriggerEnter2D(Collider2D other)
    {
        if(transform.parent)
            transform.parent.SendMessageUpwards("OnTriggerEnter2D", other, SendMessageOptions.DontRequireReceiver);
    }
}
