#include "ThinkerLib.h"
#include "World.h"
#include "Pacman.h"
#include "Blinky.h"
#include "Inky.h"
#include "Pinky.h"
#include "Clyde.h"
#include "Pacman3.h"

int CreateWorld()
{
	try
	{
		return World::CreateWorld();
	}
	catch (std::exception& e)
	{
		return -1;
	}
}

void CreateNode(int in_worldId, int in_id, NativeVector2 in_position)
{
	try
	{
		World* world = World::GetWorld(in_worldId);
		if (!world)
			return;

		world->CreateNode(in_id, in_position);
	}
	catch (std::exception& e)
	{
	}
}

void CreateConnection(int in_worldId, int in_fromId, int in_toId, int in_direction, float in_cost)
{
	try
	{
		World* world = World::GetWorld(in_worldId);
		if (!world)
			return;

		PathNode* node = world->GetNode(in_fromId);
		if (!node)
			return;

		PathNodeConnection& connection = node->GetEditableConnection(static_cast<Direction::Enum>(in_direction));
		connection = PathNodeConnection(in_toId, in_cost);
	}
	catch (std::exception& e)
	{
	}
}

void DestroyWorld(int in_worldId)
{
	World::DestroyWorld(in_worldId);
}

void CreatePointObj(int in_worldId, int in_nodeId, int in_worth, int in_type)
{
	World* world = World::GetWorld(in_worldId);
	if (!world)
		return;

	world->CreatePointObj(in_nodeId, static_cast<PointObj::Type::Enum>(in_type), in_worth);
}

void DestroyPointObj(int in_worldId, int in_nodeId)
{
	World* world = World::GetWorld(in_worldId);
	if (!world)
		return;

	world->DestroyPointObj(in_nodeId);
}

void SetPacman(int in_worldId, const NativePawn& in_pawn)
{
	World* world = World::GetWorld(in_worldId);
	if (!world)
		return;

	world->SetPacman(in_pawn);
}

void SetBlinky(int in_worldId, const NativePawn& in_pawn)
{
	World* world = World::GetWorld(in_worldId);
	if (!world)
		return;

	world->SetBlinky(in_pawn);
}

void SetPinky(int in_worldId, const NativePawn& in_pawn)
{
	World* world = World::GetWorld(in_worldId);
	if (!world)
		return;

	world->SetPinky(in_pawn);
}

void SetInky(int in_worldId, const NativePawn& in_pawn)
{
	World* world = World::GetWorld(in_worldId);
	if (!world)
		return;

	world->SetInky(in_pawn);
}

void SetClyde(int in_worldId, const NativePawn& in_pawn)
{
	World* world = World::GetWorld(in_worldId);
	if (!world)
		return;

	world->SetClyde(in_pawn);
}

int Test(int a, int b, NativeVector2 in_test)
{
	return static_cast<int>(in_test.x);
}

int ThinkPacman(int in_worldId, float in_deltaTime, float in_totalTime)
{
	World* world = World::GetWorld(in_worldId);
	if (!world)
		return -1;

	const Pawn& pacman = world->GetPacman();

	return static_cast<int>(Pacman::OnPacmanThink(pacman, *world, in_deltaTime, in_totalTime));
}

int ThinkPacman2(int in_worldId, float in_deltaTime, float in_totalTime)
{
	World* world = World::GetWorld(in_worldId);
	if (!world)
		return -1;

	const Pawn& pacman = world->GetPacman();

	return static_cast<int>(Pacman2::OnPacmanThink(pacman, *world, in_deltaTime, in_totalTime));
}

int ThinkPacman3(int in_worldId, float in_deltaTime, float in_totalTime)
{
	World* world = World::GetWorld(in_worldId);
	if (!world)
		return -1;

	const Pawn& pacman = world->GetPacman();

	return static_cast<int>(Pacman3::OnThinkPacman3(pacman, *world, in_deltaTime, in_totalTime));
}


int ThinkBlinky(int in_worldId, float in_deltaTime, float in_totalTime)
{
	World* world = World::GetWorld(in_worldId);
	if (!world)
		return Direction::Invalid;

	const Pawn& blinky = world->GetBlinky();

	return Blinky::OnBlinkyThink(blinky, *world, in_deltaTime, in_totalTime);
}

int ThinkInky(int in_worldId, float in_deltaTime, float in_totalTime)
{
	World* world = World::GetWorld(in_worldId);
	if (!world)
		return Direction::Invalid;

	const Pawn& inky = world->GetInky();

	return Inky::OnInkyThink(inky, *world, in_deltaTime, in_totalTime);
}

int ThinkPinky(int in_worldId, float in_deltaTime, float in_totalTime)
{
	World* world = World::GetWorld(in_worldId);
	if (!world)
		return Direction::Invalid;

	const Pawn& pinky = world->GetPinky();

	return Pinky::OnPinkyThink(pinky, *world, in_deltaTime, in_totalTime);
}

int ThinkClyde(int in_worldId, float in_deltaTime, float in_totalTime)
{
	World* world = World::GetWorld(in_worldId);
	if (!world)
		return Direction::Invalid;

	const Pawn& clyde = world->GetClyde();

	return Clyde::OnClydeThink(clyde, *world, in_deltaTime, in_totalTime);
}