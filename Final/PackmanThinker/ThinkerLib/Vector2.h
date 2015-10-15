#pragma once
#include <math.h>
#include "Direction.h"

struct NativeVector2
{
	float x;
	float y;
};

struct Vector2
{
	float x;
	float y;

	Vector2(float in_x, float in_y)
	{
		x = in_x;
		y = in_y;
	}

	Vector2(const Vector2& in_other)
		: x(in_other.x),
		  y(in_other.y)
	{
	}

	Vector2(const NativeVector2& in_other)
		: x(in_other.x),
		  y(in_other.y)
	{
	}

	Vector2(Direction::Enum in_direction)
	{
		switch(in_direction)
		{
		case Direction::Up:
			x = 0;
			y = 1;
			break;
		case Direction::Right:
			x = 1;
			y = 0;
			break;
		case Direction::Down:
			x = 0;
			y = -1;
			break;
		case Direction::Left:
			x = -1;
			y = 0;
			break;
		case Direction::Invalid:
		default:
			x = 0;
			y = 0;
			break;
		}
	}

	float Magnitude()
	{
		return static_cast<float>(sqrt(x * x + y * y));
	}

	float MagnitudeSqr()
	{
		return x * x + y * y;
	}

	Vector2 GetNormalizedVector()
	{
		float magintude = Magnitude();
		return Vector2(x / magintude, y / magintude);
	}

	Vector2 operator =(const Vector2& in_other)
	{
		x = in_other.x;
		y = in_other.y;
		return *this;
	}

	Vector2 operator +(const Vector2& b) const
	{
		return Vector2(x + b.x, y + b.y);
	}

	Vector2 operator -(const Vector2& b) const
	{
		return Vector2(x - b.x, y - b.y);
	}

	float operator *(const Vector2& b) const
	{
		return x * b.x + y * b.y;
	}

	Vector2 operator *(float b) const
	{
		return Vector2(x * b, y * b);
	}

	bool operator ==(const Vector2& b) const {
		return (x == b.x) && (y == b.y);
	}
};
