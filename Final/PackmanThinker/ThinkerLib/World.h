#pragma once
#include <vector>
#include <map>
#include "Vector2.h"
#include "Direction.h"
#include "Pawn.h"
#include <mutex>

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

protected:
	friend class World;

	PointObj(int in_nodeId, Type::Enum in_type, int in_worth, const Vector2& in_location);
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
	friend void CreateConnection(int in_worldId, int in_fromId, int in_toId, int in_direction, float in_cost);

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
	static int CreateWorld();
	static World* GetWorld(int id);
	static void DestroyWorld(int id);
public:
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
	friend int CreateWorld();
	friend void CreateNode(int in_worldId, int in_id, NativeVector2 in_position);
	friend void CreateConnection(int in_worldId, int in_fromId, int in_toId, int in_direction, float in_cost);
	friend void DestroyWorld(int in_worldId);
	friend void CreatePointObj(int in_worldId, int in_nodeId, int in_worth, int in_type);
	friend void DestroyPointObj(int in_worldId, int in_nodeId);
	friend void SetPacman(int in_worldId, const NativePawn& in_pawn);
	friend void SetBlinky(int in_worldId, const NativePawn& in_pawn);
	friend void SetPinky(int in_worldId, const NativePawn& in_pawn);
	friend void SetInky(int in_worldId, const NativePawn& in_pawn);
	friend void SetClyde(int in_worldId, const NativePawn& in_pawn);

	PathNode* CreateNode(int in_id, const Vector2& in_position);
	PointObj* CreatePointObj(int in_nodeId, PointObj::Type::Enum in_type, int in_cost);

	void DestroyPointObj(int in_nodeId);

	void SetPacman(const Pawn& in_pawn);
	void SetInky(const Pawn& in_pawn);
	void SetPinky(const Pawn& in_pawn);
	void SetBlinky(const Pawn& in_pawn);
	void SetClyde(const Pawn& in_pawn);
private:
	World();
	~World();

	std::map<int, PathNode*> m_nodes;
	std::map<int, PointObj*> m_pointObjs;

	static std::map<int, World*> m_worlds;
	static std::mutex m_worldLock;
	static int m_worldCount;

	Pawn m_pacman, m_inky, m_pinky, m_blinky, m_clyde;

	std::recursive_mutex m_lock;
};