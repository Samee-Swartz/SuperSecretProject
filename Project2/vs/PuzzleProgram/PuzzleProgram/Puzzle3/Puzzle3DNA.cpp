#include <string>
#include <time.h>
#include "Puzzle3DNA.h"
#include <sstream>
#include <vector>

std::vector<TowerPiece> Puzzle3DNA::m_validPieces;

// randomly creates DNA
void Puzzle3DNA::Generate() {
	std::vector<TowerPiece> copyValidDNA = m_validPieces;
	// randomly chooses how many blocks to contain. Must be 2 < x < m_validPieces.size()
	int pieces = (rand() % (m_validPieces.size()-2)) +2;

	while (pieces > 0) {
		int index = rand() % copyValidDNA.size();
		m_pieces.push_back(copyValidDNA[index]);
		copyValidDNA.erase(copyValidDNA.begin()+index);
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
	const Puzzle3DNA* parent1 = static_cast<const Puzzle3DNA*>(GetParent1());
	const Puzzle3DNA* parent2 = static_cast<const Puzzle3DNA*>(GetParent2());
	int i, coinFlip;
	for (i = 0; i < parent1->GetTowerHeight() && i < parent2->GetTowerHeight(); i++) {
		coinFlip = rand() % 2;
		if (coinFlip == 0) {
			m_pieces.push_back(parent1->GetPieceAt(i));
		} else {
			m_pieces.push_back(parent2->GetPieceAt(i));
		}
	}
	while (i < parent1->GetTowerHeight()) {
		coinFlip = rand() % 2;
		if (coinFlip == 0)
			m_pieces.push_back(parent1->GetPieceAt(i));
		i++;
	}
	while (i < parent2->GetTowerHeight()) {
		coinFlip = rand() % 2;
		if (coinFlip == 0)
			m_pieces.push_back(parent2->GetPieceAt(i));
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

	for (size_t i = 0; i < m_pieces.size(); i++) {
		auto it = m_pieces.begin() + i;
		// binary search
		int first = 0;
		int last = copyValidDNA.size();
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
			i--;
		}
	}

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

std::string Puzzle3DNA::ToString() const {
	std::stringstream s;
	s << "Type\twidth\tstrength\tcost" << std::endl;
	for ( auto p : m_pieces) {
		s << p.GetType() << "\t\t" << p.m_width << "\t\t" << p.m_strength << "\t\t" << p.m_cost << std::endl;
	}
	return s.str();
}
