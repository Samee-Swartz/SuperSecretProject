#ifndef _PUZZLE_2_DNA
#define _PUZZLE_2_DNA

#include "../Core/DNA.h"
#include <string>
#include <vector>
#include <algorithm>

class Puzzle2DNA : public DNA {
public:

	Puzzle2DNA();

	void Generate();
	void Splice();
	void Mutate();

	int getBin1Val() const;
	int getBin2Val() const;

	int getBin1At(int) const;
	int getBin2At(int) const;
	int getBin3At(int) const;

	static void SetValidPieces(const std::vector<int> in_pieces) {
		m_validPieces = in_pieces;
		std::sort(m_validPieces.begin(), m_validPieces.end());
	}

private:

	std::vector<int> m_bin1;
	std::vector<int> m_bin2;
	std::vector<int> m_bin3;

	static std::vector<int> m_validPieces;
	std::vector<int> m_invalidPieces;

	void swapValues(int validIndex1, int validIndex2);
	bool binAtIndexFull(int);

protected:
	bool isValid();

};

#endif
