#ifndef _PUZZLE_2_DNA
#define _PUZZLE_2_DNA

#include "DNA.h"
#include <string>

class Puzzle2DNA : public DNA {
public:
	
	Puzzle2DNA();
	
	void Generate();
	void Splice();
	void Mutate();
	
	int getBin1Val();
	int getBin2Val();
	
	static void SetValidPieces(std::vector<int> in_pieces);
	
private:
	
	std::vector<int> m_bin1;
	std::vector<int> m_bin2;
	std::vector<int> m_bin3;
	
	std::vector<int> m_validPieces;
	std::vector<int> m_invalidPieces;
	
	void swapValues();
	bool binAtIndexFull(int);
	
protected:
	bool isValid();
	
};

#endif
