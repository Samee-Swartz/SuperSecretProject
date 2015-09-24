//#include "DNA.h"
#include <string>
#include <time.h>
#include "Puzzle1DNA.h"

std::vector<int> Puzzle1DNA::m_validPieces;
int Puzzle1DNA::m_target = 0;

Puzzle1DNA::Puzzle1DNA(){}

void Puzzle1DNA::Generate(){
	std::vector<int> copyValidDNA = m_validPieces;

	int pieces = ((rand() % (m_validPieces.size() - 2)) + 2);

	while(pieces > 0){
		int index = rand() % m_validPieces.size();
		m_pieces.push_back(m_validPieces[index]);
		pieces--;
	}
}

void Puzzle1DNA::Splice() {
	const Puzzle1DNA* parent1 = static_cast<const Puzzle1DNA*>(GetParent1());
	const Puzzle1DNA* parent2 = static_cast<const Puzzle1DNA*>(GetParent2());

	int i, coinFlip;
	for (i = 0; i < parent1->GetSize() && i < parent2->GetSize(); i++) {
		coinFlip = rand() % 2;
		if (coinFlip == 0) {
			m_pieces.push_back(parent1->GetPieceAt(i));
		} else {
			m_pieces.push_back(parent2->GetPieceAt(i));
		}
	}
	while (i < parent1->GetSize()) {
		coinFlip = rand() % 2;
		if (coinFlip == 0)
			m_pieces.push_back(parent1->GetPieceAt(i));
		i++;
	}
	while (i < parent2->GetSize()) {
		coinFlip = rand() % 2;
		if (coinFlip == 0)
			m_pieces.push_back(parent2->GetPieceAt(i));
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

int Puzzle1DNA::GetSum() const {
	int sum = 0;

	for(int i=0; i < m_pieces.size(); i++)
		sum += m_pieces[i];

	if(sum <= m_target)
		return sum;

	return 0;
}
