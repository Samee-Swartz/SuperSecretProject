#ifndef _PUZZLE_1_DNA
#define _PUZZLE_1_DNA

#include "DNA.h"
#include <string>

class Puzzle1DNA : public DNA {
public:
	Puzzle1DNA(const Puzzle1DNA& in_source1, const Puzzle1DNA& in_source2,
	        const vector<int> in_validPieces);
	int GetPieceAt(int i){
		return m_pieces[i];
	}

	void Splice();

	void Mutate();

private:
	// list of numbers in DNA
	vector<int> m_pieces;

	// list of valid numbers for DNA
	vector<intt> m_validPieces;

};

#endif
