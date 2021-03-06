#pragma once
#include "Vector2.h"
#include "Direction.h"

struct PawnId
{
	enum Enum
	{
		None = -1,
		Pacman,
		Blinky,
		Pinky,
		Inky,
		Clyde
	};
};

struct NativePawn
{
	//The position the pawn is in
	NativeVector2 m_position;

	int m_atNode;

	//The node that we are closest too
	int m_closestNode;

	//The nodes we are closest
	int m_nextNodes[4];

	//Speed that the pawn is traveling at
	float m_speed;

	//The direction this pawn is facing in (also traveling in)
	int m_facingDirection;

	//The state code that this pawn is in
	int m_state;
};

struct Pawn
{
	Pawn();

	Pawn(const Vector2& in_position,int in_atNode , int in_closestNode, int in_nextNodes[4], float in_speed, Direction::Enum in_facingDirection, int in_state, bool in_canSsee);

	Pawn(const NativePawn& in_other);

	Vector2 GetPosition() const { return m_position; }

	int GetClosestNode() const { return m_closestNode; }

	int GetCurrentNode() const { return m_atNode; }

	void GetNextNodes(int* out_nodes) const;

	float GetSpeed() const { return m_speed; }

	Direction::Enum GetFacingDirection() const { return m_facingDirection; }

	// ghosts -1 is frightened (blue), 1 is chase, 2 is scatter (not chasing)
	// pacman 1 is alive, 0 is dead
	int GetState() const { return m_state; }

	bool CanSee() const { return m_canSee; }
private:
	//The position the pawn is in
	Vector2 m_position;

	int m_atNode;

	//The node that we are closest too
	int m_closestNode;

	//The nodes we are closest
	int m_nextNodes[4];

	//Speed that the pawn is traveling at
	float m_speed;

	//The direction this pawn is facing in (also traveling in)
	Direction::Enum m_facingDirection;

	//The state code that this pawn is in
	int m_state;

	//Do you have a direct line of sight to them (always true for your own pawn)
	bool m_canSee;
};

