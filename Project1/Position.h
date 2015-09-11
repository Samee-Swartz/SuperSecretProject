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

	bool equals(struct Position p) {
		return x == p.x && y == p.y;
	}
};
typedef struct Position Position;

#endif