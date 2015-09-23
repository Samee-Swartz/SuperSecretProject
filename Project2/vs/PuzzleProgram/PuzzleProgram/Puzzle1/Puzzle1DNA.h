#ifndef _PUZZLE_1_DNA
#define _PUZZLE_1_DNA

#include "DNA.h"
#include <string>

class Puzzle1DNA : public DNA {
public:
	Puzzle1DNA(const Puzzle1DNA& in_source1, const Puzzle1DNA& in_sources);
	int GetPieceAt(int i){
		return m_pieces[i];
	}

	Puzzle1DNA();

	void Splice();

	void Mutate();

	static void SetValidPieces(vector<int> in_pieces){
	        std::sort(m_validPieces.begin(), m_validPieces.end());
		m_validPieces = in_pieces;
	}


private:
	// list of numbers in DNA
	vector<int> m_pieces;

	// list of valid numbers for DNA
	vector<intt> m_validPieces;

};

#endif
