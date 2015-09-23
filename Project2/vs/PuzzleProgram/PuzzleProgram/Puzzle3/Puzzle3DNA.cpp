#include "DNA.h"
#include <string>
#include <time.h>
#include "Puzzle3DNA.h"

// randomly creates DNA
void Puzzle3DNA::Generate() {
	std::vector<TowerPiece> copyValidDNA = m_validPieces;
	// randomly chooses how many blocks to contain. Must be 2 < x < m_validPieces.size()
	int pieces = (rand() % (m_validPieces.size()-2)) +2;

	while (pieces > 0) {
		int index = rand() % m_validPieces.size();
		m_pieces.push_back(m_validPieces[index]);
		pieces--;
	}
}

// Takes two parents and splices their DNA together into this child
/* 	Algorithm:
	The program runs through the index's of the parent's DNA (until the shorter of the two)
	and flips a coin to decide which parent's TowerPiece (at that index) it should choose.
	After the the shorter of the parents' DNAs ran out, it flips a coin to decide whether
	or not to to take that index's TowerPiece.
*/
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

// Mutates this child's DNA
/* 	Algorithm:
	The program runs through the valid DNA and verifies that each piece is only
	used once, removing duplicates along the way.
	The program then randomly chooses a number between 0 and how many duplicates were removed
	and randomly inserts unused validPieces into the DNA vector.
*/
void Puzzle3DNA::Mutate() {
	std::vector<TowerPiece> copyValidDNA = m_validPieces;
	int numDups = 0;

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
            numDups++;
            --it; // used to compensate for removing in the middle of the for loop
		}
	}
	// could add extra mutate here

	if (numDups > 0) {
		int numAdds = rand() % numDups;
		while (numAdds > 0) {
			int validIndex = rand() % copyValidDNA.size();
			int piecesIndex = rand() % m_pieces.size();
			m_pieces.insert(m_pieces.begin()+piecesIndex, copyValidDNA[validIndex]);
			numAdds--;
		}
	}
}


