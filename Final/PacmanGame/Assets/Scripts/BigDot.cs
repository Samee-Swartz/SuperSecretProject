using UnityEngine;
using System.Collections;

public class BigDot : ScoreNode
{
    protected override void OnPickup()
    {
        Game.Instance.FrightenGhosts();
    }
}
