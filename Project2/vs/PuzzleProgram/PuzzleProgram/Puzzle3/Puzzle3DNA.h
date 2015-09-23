#ifndef _PUZZLE_3_DNA
#define _PUZZLE_3_DNA

#include "../Core/DNA.h"
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
	TowerPiece() {}

	bool operator==(const TowerPiece& other) const {
		return m_type == other.m_type && m_width == other.m_width &&
			m_strength == other.m_strength && m_cost == other.m_cost;
	}
};

class Puzzle3DNA : public DNA {
public:
	Puzzle3DNA(){}

	TowerPiece GetPieceAt(int in_index) { return m_pieces[in_index];}

	int GetTowerHeight() {return m_pieces.size();}

	// Returns tower's total cost
	int GetTowerCost() {
		int cost = 0;
		for (auto p : m_pieces) {
			cost += p.m_cost;
		}
		return cost;
	}

	// checks if the bottom TowerPiece is a Door
	bool IsDoorOnBottom() {
		if (m_pieces.front().m_type == DOOR)
			return true;
		return false;
	}
	// checks if the top TowerPiece is a Lookout
	bool IsLookoutOnTop() {
		if (m_pieces.back().m_type == LOOKOUT)
			return true;
		return false;
	}
	// checks if the middle TowerPieces are Walls
	bool AreWallsInMiddle() {
		for (int i = 1; i < m_pieces.size() - 1; i++) { // skips the top and bottom
			if (m_pieces[i].m_type != WALL)
				return false;
		}
		return true;
	}
	// checks that the widths are valid
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
	// checks that the strengths are valid
	bool VerifyStrength() {
		int remainingSize = m_pieces.size();
		for (auto p : m_pieces) {
			remainingSize--;
			if (p.m_strength < remainingSize)
				return false;
		}
		return true;
	}
	// static setter for the valid pieces list
	static void SetValidPieces(std::vector<TowerPiece> in_pieces) {
		std::sort(m_validPieces.begin(), m_validPieces.end(), compareTowerPieces);
		m_validPieces = in_pieces;
	}

private:
	// element at index 0 is bottom of tower.
	std::vector<TowerPiece> m_pieces;
	// sorted list of valid Tower Pieces
	static std::vector<TowerPiece> m_validPieces;
	// comparator for TowerPieces
	static bool compareTowerPieces(const TowerPiece& p1, const TowerPiece& p2) {
		return p1.m_width + p1.m_strength + p1.m_cost < p2.m_width + p2.m_strength + p2.m_cost;
	}
};

#endif
