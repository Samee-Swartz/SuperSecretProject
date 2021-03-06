//Class declaration for CellDataClass
#include "Position.h"
#include "RobotState.h"
#include "ModData.h"
#include "World.h"

CellData::CellData()
	: p(0,0),
	terrain(0)
{
}

CellData::CellData(const Position pos, const int terr)
	: p(pos),
	terrain(terr)
{
}

Mod::Mod(const Position pos)
{
	Position northWestPos = Position(pos.x - 1, pos.y + 1);
	northWest = CellData(northWestPos, World::getInstance().getTerrain(northWestPos));

	Position northPos = Position(pos.x, pos.y + 1);
	north = CellData(northPos, World::getInstance().getTerrain(northPos));

	Position northEastPos = Position(pos.x + 1, pos.y + 1);
	northEast = CellData(northEastPos, World::getInstance().getTerrain(northEastPos));

	Position westPos = Position(pos.x - 1, pos.y);
	west = CellData(westPos, World::getInstance().getTerrain(westPos));

	Position eastPos = Position(pos.x + 1, pos.y);
	east = CellData(eastPos, World::getInstance().getTerrain(eastPos));

	Position southWestPos = Position(pos.x - 1, pos.y - 1);
	southWest = CellData(southWestPos, World::getInstance().getTerrain(southWestPos));

	Position southPos = Position(pos.x, pos.y - 1);
	south = CellData(southPos, World::getInstance().getTerrain(southPos));

	Position southEastPos = Position(pos.x + 1, pos.y - 1);
	southEast = CellData(southEastPos, World::getInstance().getTerrain(southEastPos));

	destroyWorld();
}

void Mod::destroyWorld() {
	World& w = World::getInstance();
	w.setTerrain(northWest.getPosition(), 3);
	w.setTerrain(north.getPosition(), 3);
	w.setTerrain(northEast.getPosition(), 3);
	w.setTerrain(west.getPosition(), 3);
	w.setTerrain(east.getPosition(), 3);
	w.setTerrain(southWest.getPosition(), 3);
	w.setTerrain(south.getPosition(), 3);
	w.setTerrain(southEast.getPosition(), 3);
}

void Mod::restoreWorld() {
	World& w = World::getInstance();
	w.setTerrain(northWest.getPosition(), northWest.getTerrain());
	w.setTerrain(north.getPosition(), north.getTerrain());
	w.setTerrain(northEast.getPosition(), northEast.getTerrain());
	w.setTerrain(west.getPosition(), west.getTerrain());
	w.setTerrain(east.getPosition(), east.getTerrain());
	w.setTerrain(southWest.getPosition(), southWest.getTerrain());
	w.setTerrain(south.getPosition(), south.getTerrain());
	w.setTerrain(southEast.getPosition(), southEast.getTerrain());
}
