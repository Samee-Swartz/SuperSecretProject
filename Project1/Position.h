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
	Position() {
		x = 0;
		y = 0;
	}

	bool equals(const Position& p) const {
		return x == p.x && y == p.y;
	}

	bool operator==(const Position& other) const
	{
		return equals(other);
	}

	Position operator=(const Position& other) const {
		return Position(other.x, other.y);
	}

	void setValues(int x, int y) {
		x = x;
		y = y;
	}
};

#endif
