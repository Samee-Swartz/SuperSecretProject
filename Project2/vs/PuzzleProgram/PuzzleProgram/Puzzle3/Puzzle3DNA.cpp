#include "DNA.h"
#include <string>

struct TowerPiece {
	std::string m_type; // either door, wall, or lookout
	int m_width; // how wide. must be > 0
	int m_strength; // how many pieces can be stacked on top of it. must be >0
	int m_cost; // how expensive is it to use. must be > 0

	TowerPiece(std::string in_type, int in_width, int in_strength, int in_cost) {
		m_width = in_width;
		m_type = in_type;
		m_strength = in_strength;
		m_cost = in_cost;
	}
};

class Puzzle3DNA : public DNA {
public:
	Puzzle3DNA(const Puzzle3DNA& in_source1, const Puzzle3DNA& in_source2)
		: DNA(in_source1, in_source2) {
			Splice();
			Mutate();
	}

	void Splice() {
		// splices parents into kid
	}

	void Mutate() {
		// mutates kid
	}

	vector<TowerPiece> GetPieces() {return m_pieces;}

private:
	// element at index 0 is bottom of tower.
	vector<TowerPiece> m_pieces;
};

