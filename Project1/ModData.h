#ifndef _MODDATA_
#define _MODDATA_

// Position and Terrain information about a single world cell
class CellData{
	Position p;
	int terrain;
  public:
    CellData(const Position position, const int terrain);
	Position getPosition() {return p;}
	int getTerrain() {return terrain;}
};

// Stores information about 8 world cells that have been demolished
class Mod{
	CellData northWest;
	CellData north;
	CellData northEast;
	CellData west;
	CellData east;
	CellData southWest;
	CellData south;
	CellData southEast;

  public:
	Mod(const Position);
	restoreWorld();
	destroyWorld();
};

#endif
