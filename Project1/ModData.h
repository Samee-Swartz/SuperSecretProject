//Header file for the Mod Class and the Cell Data Class

struct Position {
	int x;
	int y;
	Position(int x, int y) {
		x = x;
		y = y;
	}
};

class CellData{
	struct Position p;
	int terrain;
  public:
    CellData(struct position, int);
	getPosition();
	getTerrain();
};

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
	Mod(CellData);
	Mod(struct Position);
	restoreWorld();
};
