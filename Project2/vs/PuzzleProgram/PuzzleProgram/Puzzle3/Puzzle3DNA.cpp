#include "DNA.h"
#include <string>

enum PieceType { DOOR, WALL, LOOKOUT };

struct TowerPiece {
	PieceType m_type; // either door, wall, or lookout
	int m_width; // how wide. must be > 0
	int m_strength; // how many pieces can be stacked on top of it. must be >0
	int m_cost; // how expensive is it to use. must be > 0

	TowerPiece(const PieceType& in_type, int in_width, int in_strength, int in_cost) {
		if (in_width > 0)
			m_width = in_width;
		else
			m_width = 1;

		if (in_strength > 0)
			m_strength = in_strength;
		else
			m_strength = 1;

		if (in_cost > 0)
			m_cost = in_cost;
		else
			m_cost = 1;

		m_type = in_type;
	}
	TowerPiece() {};

	bool operator==(const TowerPiece& other) const {
		return m_type == other.m_type && m_width == other.m_width &&
			m_strength == other.m_strenth && m_cost == other.m_cost;
	}
};

class Puzzle3DNA : public DNA {
public:
	Puzzle3DNA(const Puzzle3DNA& in_source1, const Puzzle3DNA& in_source2,
	        const vector<TowerPiece> in_validPieces)
			: DNA(in_source1, in_source2), m_validPieces(in_validPieces) {
		Splice();
		Mutate();
	}

	void Splice() {
		// splices parents into kid
		// use m_parent1 and m_parent2
	}

	void Mutate() {
		// mutates kid
		// validates kid

		// vector<TowerPiece> copyDNA = m_pieces;
		// for (auto vp : m_validPieces) {
		// 	for (std::vector<TowerPiece>::iterator it = copyDNA.begin() ; it != copyDNA.end(); ++it) {
		// 		if ((*it)==vp) {
		// 			copyDNA.erase(it, it+1);
		// 			break;
		// 		}
		// 	}
		// }
		// if copyDNA
	}

	int GetTowerHeight() {return m_pieces.size();}
	int GetTowerCost() {
		int cost = 0;
		for (auto p : m_pieces) {
			cost += p.m_cost;
		}
		return cost;
	}

	bool IsDoorOnBottom() {
		if (m_pieces.front().m_type == DOOR)
			return true;
		return false;
	}
	bool IsLookoutOnTop() {
		if (m_pieces.back().m_type == LOOKOUT)
			return true;
		return false;
	}
	bool AreWallsInMiddle() {
		for (int i = 1; i < m_pieces.size() - 1; i++) { // skips the top and bottom
			if (m_pieces[i].m_type != WALL)
				return false;
		}
		return true;
	}
	bool VerifyWidths() {
		int cur_width = m_pieces.front().m_width;
		for (auto p : m_pieces) {
			if (p.m_width < cur_width)
				cur_width = p.m_width;
			else if (p.m_width > cur_width) // unstable tower!
				return false;
		}
		return true;
	}
	bool VerifyStrength() {
		int remainingSize = m_pieces.size();
		for (auto p : m_pieces) {
			remainingSize--;
			if (p.m_strength < remainingSize)
				return false;
		}
		return true;
	}

private:
	// element at index 0 is bottom of tower.
	vector<TowerPiece> m_pieces;
	vector<TowerPiece> m_validPieces;
};

