//Class declaration for CellDataClass
#include RobotState.h
#include ModData.h
#include World.h

CellData::CellData(struct Position pos, int terr){
	p = pos;
	terrain = terr;
}

Mod::Mod(CellData aCell){
	northWestPos = Position(getPosition().x - 1, getPosition().y + 1);
	northWest = CellData::CellData(northWestPos, World.getInstance().getTerrain(northWestPos));
	nortPos = Position(getPosition().x, getPosition().y + 1)
	north = CellData::CellData(northPos, World.getInstance().getTerrain(northPos));
	northEastPos = Position(getPosition().x + 1, getPosition().y + 1)
	northEast = CellData::CellData(northEastPos, World.getInstance().getTerrain(northEastPos));
	westPos = Position(getPosition().x - 1, getPosition().y);
	west = CellData::CellData(westPos, World.getInstance().getTerrain(westPos));
	eastPos = Position(getPosition().x + 1, getPosition().y);
	east = CellData::CellData(eastPos, World.getInstance().getTerrain(eastPos));
	southWestPos = Position(getPosition().x - 1, getPosition().y - 1)
	southWest = CellData::CellData(southWestPos, World.getInstance().getTerrain(southWestPos));
	southPos = Position(getPosition().x, getPosition().y - 1);
	south = CellData::CellData(southPos, World.getInstance().getTerrain(southPos));
	southEastPos = Position(getPosition().x + 1, getPosition().y - 1);
	southEast = CellData::CellData(southEastPos, World.getInstance().getTerrain(southEastPos));
	
	destuct();
}

struct Position getPosition(){
	return p;
}

int getTerrain(){
	return terrain;
}

void Mod(cellInfo){
	


	Mod(CellData);
	Mod(struct Position);
	restoreWorld();

