#include "World.h"

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

PathNodeConnection& PathNode::GetEditableConnection(Direction::Enum in_direction)
{
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