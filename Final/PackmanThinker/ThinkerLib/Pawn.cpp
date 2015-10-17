#include "Pawn.h"

Pawn::Pawn()
	: m_position(0, 0),
	m_closestNode(-1),
	m_speed(0),
	m_facingDirection(Direction::Invalid),
	m_state(-1),
	m_canSee(false)
{
	for (int i = 0; i < sizeof(m_nextNodes) / sizeof(int); i++)
	{
		m_nextNodes[i] = -1;
	}
}

Pawn::Pawn(const Vector2& in_position, int in_atNode, int in_closestNode, int in_nextNodes[4], float in_speed, Direction::Enum in_facingDirection, int in_state, bool in_canSsee)
	: m_position{in_position},
	  m_atNode{in_atNode},
	  m_closestNode{in_closestNode},
	  m_speed{in_speed},
	  m_facingDirection{in_facingDirection},
	  m_state{in_state},
	  m_canSee{in_canSsee}
{
	for (int i = 0; i < sizeof(m_nextNodes) / sizeof(int); i++)
	{
		m_nextNodes[i] = in_nextNodes[i];
	}
}

Pawn::Pawn(const NativePawn& in_other)
	: m_position{ in_other.m_position }
{
	m_closestNode = in_other.m_closestNode;
	m_atNode = in_other.m_atNode;
	
	for (int i = 0; i < sizeof(m_nextNodes) / sizeof(int); i++)
	{
		m_nextNodes[i] = in_other.m_nextNodes[i];
	}

	m_speed = in_other.m_speed;
	m_facingDirection = static_cast<Direction::Enum>(in_other.m_facingDirection);
	m_state = in_other.m_state;
	m_canSee = false;
}

void Pawn::GetNextNodes(int* out_nodes) const
{
	for (int i = 0; i < 4; i++)
	{
		out_nodes[i] = m_nextNodes[i];
	}
}