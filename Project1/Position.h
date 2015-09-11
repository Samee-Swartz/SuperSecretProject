#ifndef _POSITION_
#define _POSITION_

// (x,y) coordinate position
struct Position {
	int x;
	int y;
	Position(int in_x, int in_y) {
		x = in_x;
		y = in_y;
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

	void setValues(int x, int y) {
		x = x;
		y = y;
	}
};

#endif
