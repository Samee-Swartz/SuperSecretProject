using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using UnityEngine.UI;

[RequireComponent(typeof(LayoutGroup))]
public class CounterBar : MonoBehaviour
{
    void Start()
    {
        if (transform.childCount <= 0)
            return;

        for (int i = 0; i < transform.childCount; i++)
        {
            RectTransform rectChild = transform.GetChild(i) as RectTransform;
            if (rectChild)
                m_children.Add(rectChild);
        }

        SetValue(m_value);
    }

    public void SetValue(int value)
    {
        int difference = value - m_value;
        if(difference > 0)
            CountUp(value);
        else if (difference < 0)
            CountDown(value);
    }

    private void CountUp(int target)
    {
        for (; m_value < target; m_value++)
        {
            if (m_value >= m_children.Count)
            {
                RectTransform newChild = Instantiate(m_children[0]);
                newChild.parent = transform;
                m_children.Add(newChild);
            }
            else
                m_children[m_value].gameObject.SetActive(true);
        }
    }

    private void CountDown(int target)
    {
        for (; m_value > target; m_value--)
        {
            m_children[m_value].gameObject.SetActive(false);
        }
    }

    private List<RectTransform> m_children = new List<RectTransform>();

    [SerializeField]
    private int m_value;
}
