#include "Pawn.h"

Pawn::Pawn()
	: m_position(0, 0),
	m_closestNode(-1),
	m_speed(0),
	m_facingDirection(Direction::Invalid),
	m_state(-1),
	m_canSee(false)
{
}

Pawn::Pawn(const Vector2& in_position, int in_closestNode, float in_speed, Direction::Enum in_facingDirection, int in_state, bool in_canSsee)
	: m_position{in_position},
	  m_closestNode{in_closestNode},
	  m_speed{in_speed},
	  m_facingDirection{in_facingDirection},
	  m_state{in_state},
	  m_canSee{in_canSsee}
{
}

void Pawn::GetNextNodes(int* out_nodes) const
{
	for (int i = 0; i < 4; i++)
	{
		out_nodes[i] = m_nextNodes[i];
	}
}