

using System;
using UnityEngine;

public enum Direction
{
    Invalid = -1,
    Up = 0,
    Right,
    Down,
    Left
}

public static class DirectionHelper
{
    public static Vector2 DirectionToVector(Direction direction)
    {
        switch (direction)
        {
            case Direction.Invalid:
                return Vector2.zero;
            case Direction.Up:
                return Vector2.up;
            case Direction.Right:
                return Vector2.right;
            case Direction.Down:
                return Vector2.down;
            case Direction.Left:
                return Vector2.left;
            default:
                throw new ArgumentOutOfRangeException("direction", direction, null);
        }
    }

    public static Direction VectorToDirection(Vector2 vector)
    {
        vector = vector.normalized;

        float amountUp = Vector2.Dot(vector, Vector2.up);
        float amountRight = Vector2.Dot(vector, Vector2.right);
        float amountLeft = Vector2.Dot(vector, Vector2.left);
        float amountDown = Vector2.Dot(vector, Vector2.down);

        float max = Mathf.Max(amountUp, amountRight, amountLeft, amountDown);

        const float tolerance = 0.001f;
        if(Math.Abs(max - amountUp) < tolerance)
            return Direction.Up;
        if(Math.Abs(max - amountDown) < tolerance)
            return Direction.Down;
        if(Math.Abs(max - amountRight) < tolerance)
            return Direction.Right;
        if (Math.Abs(max - amountLeft) < tolerance)
            return Direction.Left;

        throw new InvalidOperationException();
    }

    public static Direction OppositeDirection(Direction direction)
    {
        int intDirection = (int) direction;

        return (Direction) ((intDirection + 2) % 4);
    }
}