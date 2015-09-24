//#include "DNA.h"
#include <string>
#include <time.h>
#include "Puzzle1DNA.h"

std::vector<int> Puzzle1DNA::m_validPieces;

//Puzzle1DNA::Puzzle1DNA(const Puzzle1DNA& in_source1, const Puzzle1DNA& in_source2,
//		const vector<int> in_validPieces)
//		: DNA(in_source1, in_source2), m_validPieces(in_validPieces) {
//	std::sort(m_validPieces.begin(), m_validPieces.end());
//
//	Splice();
//	Mutate();
//	std::sort(m_pieces.begin(), m_pieces.end());
//}

void Puzzle1DNA::Generate(){
	std:vector<int> copyValidDNA = m_validPieces;
	
	int pieces = ((rand() % m_validPieces.size() - 2) + 2);

	while(pieces > 0){
		int index = rand() % m_validPieces.size();
		m_pieces.push_back(m_validPieces[index]);
		pieces--;
	}
}

void Puzzle1DNA::Splice() {
	int i, coinFlip;
	for (i = 0; i < GetParent1().size() && i < GetParent2().size(); i++) {
		coinFlip = rand() % 2;
		if (coinFlip == 0) {
			m_pieces.push_back(GetParent1().GetPieceAt(i));
		} else {
			m_pieces.push_back(GetParent2().GetPieceAt(i));
		}
	}
	while (i < GetParent1().size()) {
		coinFlip = rand() % 2;
		if (coinFlip == 0)
			m_pieces.push_back(GetParent1().GetPieceAt(i));
		i++;
	}
	while (i < GetParent2().size()) {
		coinFlip = rand() % 2;
		if (coinFlip == 0)
			m_pieces.push_back(GetParent2().GetPieceAt(i));
		i++;
	}
}

void Puzzle1DNA::Mutate() {
	std::vector<int> copyValidDNA = m_validPieces;
	for (std::vector<int>::iterator it = m_pieces.begin(); it != m_pieces.end(); ++it) {
		// binary search
		int first = 0;
		int last = m_validPieces.size();
		int mid = (last - first)/2;
		bool found = false;
		while (true) {
			if (copyValidDNA[mid] == (*it)) {
				copyValidDNA.erase(copyValidDNA.begin()+mid);
				found = true;
				break;
			}
			if ((last - first) < 2) // not found
				break;
			// <
			if ((*it) < copyValidDNA[mid]) {
				last = mid;
			} else {  // >
				first = mid;
			}
			int temp = mid;
			mid = first + ((last - first)/2);
		}
		if (!found) {
            m_pieces.erase(it);
            --it; // used to compensate for removing in the middle of the for loop
		}
	}
	// could add extra mutate here
}


