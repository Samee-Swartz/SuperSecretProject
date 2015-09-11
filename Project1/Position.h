#ifndef _POSITION_
#define _POSITION_

// (x,y) coordinate position
struct Position {
	int x;
	int y;
	Position(int x, int y) {
		x = x;
		y = y;
	}

	bool equals(const Position& p) const {
		return x == p.x && y == p.y;
	}

	bool operator==(const Position& other) const
	{
		return equals(other);
	}
};

#endif