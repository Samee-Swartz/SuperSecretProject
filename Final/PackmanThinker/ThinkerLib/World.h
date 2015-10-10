#pragma once
#include <vector>
#include <map>
#include "Vector2.h"
#include "Direction.h"
#include "Pawn.h"

class PathNodeConnection
{
public:
	PathNodeConnection();
	explicit PathNodeConnection(int in_otherNode, float in_cost);
public:
	bool IsValid() const { return m_otherNode > -1; }

	float GetCost() const { return m_cost; }

	int GetOtherNodeId() const { return m_otherNode; }
private:
	int m_otherNode;
	float m_cost;
};

class PointObj
{
public:
	struct Type
	{
		enum Enum
		{
			Dot,
			SuperDot,
			Fruit
		};
	};

	Vector2 GetPosition() const { return m_location; }
	int GetWorth() const { return m_worth; }
	Type::Enum GetType() const { return m_type; }
	int GetNode() const { return m_nodeId; }

private:
	Vector2 m_location;
	int m_worth;
	Type::Enum m_type;
	int m_nodeId;
};

class PathNode
{
public:
	Vector2 GetPosition() const { return m_position; }
	int GetId() const { return m_id; }
	PointObj* GetObject() const { return m_object; }
	bool Equals(PathNode* in_node) const;

	const PathNodeConnection& GetConnection(Direction::Enum in_direction) const { return m_connections[in_direction]; }
protected:
	friend class World;

	PathNode(int in_id, const Vector2& m_position);

	PathNodeConnection& GetEditableConnection(Direction::Enum in_direction);
private:
	Vector2 m_position;
	PathNodeConnection m_connections[4];
	int m_id;
	PointObj* m_object;
};

class World
{
public:
	World();
	~World();

	PathNode* GetNode(int in_nodeId) const;
	PointObj* GetPointObject(int in_nodeId) const;

	void GetNodes(std::vector<PathNode*>& out_nodes) const;
	void GetPointObjects(std::vector<PointObj*>& out_nodes) const;

	const Pawn& GetPacman() const { return m_pacman; }
	const Pawn& GetInky() const { return m_inky; }
	const Pawn& GetPinky() const { return m_pinky; }
	const Pawn& GetBlinky() const { return m_blinky; }
	const Pawn& GetClyde() const { return m_clyde; }
protected:
	PathNode* CreateNode(int in_id, const Vector2& in_position);
private:
	std::map<int, PathNode*> m_nodes;
	std::map<int, PointObj*> m_pointObjs;

	Pawn m_pacman, m_inky, m_pinky, m_blinky, m_clyde;
};
