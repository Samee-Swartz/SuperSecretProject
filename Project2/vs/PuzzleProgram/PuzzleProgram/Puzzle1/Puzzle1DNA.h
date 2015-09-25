#ifndef _PUZZLE_1_DNA
#define _PUZZLE_1_DNA

#include "../Core/DNA.h"
#include <string>
#include <vector>
#include <algorithm>


class Puzzle1DNA : public DNA {
public:
	Puzzle1DNA();

	int GetPieceAt(int i) const { return m_pieces[i]; }

 	//Called when there are no parents, generates a random DNA
    void Generate();
	void Splice();
	void Mutate();

	int GetSum() const;
	int GetSize() const {return m_pieces.size();}

	static void SetValidPieces(std::vector<int> in_pieces, int in_target) {
		m_validPieces = in_pieces;
		m_target = in_target;
        std::sort(m_validPieces.begin(), m_validPieces.end());
	}

private:
	// list of numbers in DNA
	std::vector<int> m_pieces;

	// list of valid numbers for DNA
	static std::vector<int> m_validPieces;
	static int m_target;
};

#endif
