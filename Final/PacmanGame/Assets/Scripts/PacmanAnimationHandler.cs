using UnityEngine;
using System.Collections;

public class PacmanAnimationHandler : MonoBehaviour
{
    public void OnDeathAnimationFinished()
    {
        Game.Instance.ResetGame();
    }
}
