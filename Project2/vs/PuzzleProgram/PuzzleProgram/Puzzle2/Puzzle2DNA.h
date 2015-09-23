#ifndef _PUZZLE_2_DNA
#define _PUZZLE_2_DNA

#include "DNA.h"
#include <string>

class Puzzle2DNA : public DNA {
public:
	Puzzle2DNA(const Puzzle2DNA& in_source1, const Puzzle2DNA& in_source2, 
			const vector<int> in_validPieces);
	Puzzle2DNA();
	
	void Generate();
	void Splice();
	void Mutate();
	
	int getBin1Val();
	int getBin2Val();
	
	static void SetValidPieces(vector<int> in_pieces);
	
private:
	
	vector<int> m_bin1;
	vector<int> m_bin2;
	vector<int> m_bin3;
	
	vector<int> m_validPieces;
	vector<int> m_invalidPieces;
	
	void swapValues();
	
protected:
	bool isValid();
	
};

#endif
