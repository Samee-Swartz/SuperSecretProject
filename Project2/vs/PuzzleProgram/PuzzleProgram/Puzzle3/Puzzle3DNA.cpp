#include "DNA.h"
#include <string>
#include <time.h>
#include "Puzzle3DNA.h"

Puzzle3DNA::Puzzle3DNA(const Puzzle3DNA& in_source1, const Puzzle3DNA& in_source2,
		const vector<TowerPiece> in_validPieces)
		: DNA(in_source1, in_source2), m_validPieces(in_validPieces) {
	std::sort(m_validPieces.begin(), m_validPieces.end(), compareTowerPieces);
	Splice();
	Mutate();
}

void Puzzle3DNA::Splice() {
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

void Puzzle3DNA::Mutate() {
	std::vector<TowerPiece> copyValidDNA = m_validPieces;
	for (std::vector<TowerPiece>::iterator it = m_pieces.begin(); it != m_pieces.end(); ++it) {
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
			if (compareTowerPieces((*it), copyValidDNA[mid])) {
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


