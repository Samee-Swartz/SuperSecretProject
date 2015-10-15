#include "World.h"
#include <mutex>

std::map<int, World*> World::m_worlds;

PathNodeConnection::PathNodeConnection()
	: m_otherNode(-1),
	m_cost(FLT_MAX)
{
}

PathNodeConnection::PathNodeConnection(int in_otherNode, float in_cost)
	: m_otherNode(in_otherNode),
	m_cost(in_cost)
{
}

PathNode::PathNode(int in_id,const Vector2& in_postion)
	: m_position(in_postion),
	m_id(in_id)
{
}

PointObj::PointObj(int in_nodeId, Type::Enum in_type, int in_worth, const Vector2& in_location)
	: m_location(in_location),
	m_worth(in_worth),
	m_type(in_type),
	m_nodeId(in_nodeId)
{
}

bool PathNode::Equals(PathNode* in_node) const {
	return m_id == in_node->GetId() && m_position == in_node->GetPosition();
}

PathNodeConnection& PathNode::GetEditableConnection(Direction::Enum in_direction)
{
	if (in_direction > Direction::Left)
		throw std::out_of_range("Invalid direction specified");

	return m_connections[in_direction];
}

World::World()
{
}

World::~World()
{
	for(auto& entry : m_nodes)
	{
		delete entry.second;
	}
}

PathNode* World::CreateNode(int in_id, const Vector2& in_position)
{
	PathNode* newNode = new PathNode(in_id, in_position);
	m_nodes.insert(std::pair<int, PathNode*>(in_id, newNode));

	return newNode;
}

PointObj* World::CreatePointObj(int in_nodeId, PointObj::Type::Enum in_type, int in_worth)
{
	PathNode* node = GetNode(in_nodeId);
	if (!node)
		return nullptr;

	PointObj* newPointObj = new PointObj(in_nodeId, in_type, in_worth, node->m_position);
	m_pointObjs.insert(std::pair<int, PointObj*>(in_nodeId, newPointObj));

	return newPointObj;
}

void World::DestroyPointObj(int in_nodeId)
{
	auto itr = m_pointObjs.find(in_nodeId);
	if (itr == m_pointObjs.end())
		return;

	auto pair = *itr;

	m_pointObjs.erase(itr);

	PathNode* node = GetNode(in_nodeId);
	if(node)
		node->m_object = nullptr;

	delete pair.second;
}

void World::SetPacman(const Pawn& in_pawn)
{
	m_pacman = in_pawn;
}

void World::SetInky(const Pawn& in_pawn)
{
	m_inky = in_pawn;
}

void World::SetPinky(const Pawn& in_pawn)
{
	m_pinky = in_pawn;
}

void World::SetBlinky(const Pawn& in_pawn)
{
	m_blinky = in_pawn;
}

void World::SetClyde(const Pawn& in_pawn)
{
	m_clyde = in_pawn;
}

int World::CreateWorld()
{
	static int worldCount = 0;
	static std::mutex creationLock;

	std::lock_guard<std::mutex> lock(creationLock);
	
	World* world = new World();
	m_worlds.insert(std::pair<int, World*>(worldCount, world));
	int assignedWorldId = worldCount;

	worldCount++;

	return assignedWorldId;
}

World* World::GetWorld(int id)
{
	return m_worlds[id];
}

void World::DestroyWorld(int id)
{
	World* world = m_worlds[id];
	
	std::lock_guard<std::recursive_mutex> lock(world->m_lock);
	delete world;
}

PathNode* World::GetNode(int in_nodeId) const
{
	auto foundNode = m_nodes.find(in_nodeId);
	if (foundNode == m_nodes.end())
		return nullptr;

	return foundNode->second;
}

PointObj* World::GetPointObject(int in_nodeId) const
{
	auto foundNode = m_pointObjs.find(in_nodeId);
	if (foundNode == m_pointObjs.end())
		return nullptr;

	return foundNode->second;
}

void World::GetNodes(std::vector<PathNode*>& out_nodes) const
{
	for (auto& entry : m_nodes)
	{
		out_nodes.push_back(entry.second);
	}
}

void World::GetPointObjects(std::vector<PointObj*>& out_nodes) const
{
	for (auto& entry : m_pointObjs)
	{
		out_nodes.push_back(entry.second);
	}
}
