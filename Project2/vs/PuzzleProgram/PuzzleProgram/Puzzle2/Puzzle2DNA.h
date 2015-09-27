#ifndef _PUZZLE_2_DNA
#define _PUZZLE_2_DNA

#include "../Core/DNA.h"
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

class Puzzle2DNA : public DNA {
public:

	Puzzle2DNA();

	void Generate();
	void Splice();
	void Mutate();

	std::string ToString() const;

	float getBin1Val() const;
	float getBin2Val() const;

	float getBin1At(int) const;
	float getBin2At(int) const;
	float getBin3At(int) const;

	static void SetValidPieces(const std::vector<float> in_pieces) {
		m_validPieces = in_pieces;
		std::sort(m_validPieces.begin(), m_validPieces.end());
	}

private:

	std::vector<float> m_bin1;
	std::vector<float> m_bin2;
	std::vector<float> m_bin3;

	static std::vector<float> m_validPieces;
	std::vector<float> m_invalidPieces;

	void swapValues(int validIndex1, int validIndex2);
	bool binAtIndexFull(int);
	bool BinarySearch(std::vector<float>& in_validDNA, std::vector<float>::iterator it);

};

#endif
