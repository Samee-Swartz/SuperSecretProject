using UnityEngine;
using System.Collections;

public class InputController : MonoBehaviour
{
	// Update is called once per frame
	void Update ()
	{
	    MovingAgent agent = GetComponent<MovingAgent>();

	    if (Input.GetKeyDown(KeyCode.DownArrow))
	    {
	        agent.InputDirection = Direction.Down;
	    }
        else if (Input.GetKeyDown(KeyCode.UpArrow))
        {
            agent.InputDirection = Direction.Up;
        }
        else if (Input.GetKeyDown(KeyCode.LeftArrow))
        {
            agent.InputDirection = Direction.Left;
        }
        else if (Input.GetKeyDown(KeyCode.RightArrow))
        {
            agent.InputDirection = Direction.Right;
        }
	}
}
