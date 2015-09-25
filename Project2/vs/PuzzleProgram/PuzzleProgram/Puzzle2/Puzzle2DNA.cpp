#include "Puzzle2DNA.h"
#include <stdlib.h>
#include <string>

std::vector<int> Puzzle2DNA::m_validPieces;

//Constructor
Puzzle2DNA::Puzzle2DNA() {}


//Generate the DNA data for this particular creature
void Puzzle2DNA::Generate(){
	//generate random DNA from the given m_validPieces

	int i;
	int index;

	for(i = 0; i < 30; i++){
		 if(i < 10)
			 m_bin1[i] = 0;
		 else if(i < 20)
			 m_bin2[i-10] = 0;
		 else
			 m_bin3[i-20] = 0;
	}

	//for each value in m_validPieces(30), find an empty bin position and place it there.
	for(i = 0; i < 30; i ++){
		index = rand() % 30;

		if(!binAtIndexFull(index)){
			if(index < 10)
				m_bin1[index] = m_validPieces[i];
			else if(index < 20)
				m_bin2[index - 10] = m_validPieces[i];
			else
				m_bin3[index - 20] = m_validPieces[i];
		}
		else{
			while(binAtIndexFull(index)){
				index = rand() % 30;
			}

			//we now have a valid index, place the value in the available position
			if(index < 10)
				m_bin1[index] = m_validPieces[i];
			else if(index < 20)
				m_bin2[index - 10] = m_validPieces[i];
			else
				m_bin3[index - 20] = m_validPieces[i];
		}


	}

}

void Puzzle2DNA::Splice() {
	const Puzzle2DNA* p1 = static_cast<const Puzzle2DNA*>(GetParent1());
	const Puzzle2DNA* p2 = static_cast<const Puzzle2DNA*>(GetParent2());

	// splices parents into kid

	int bin1Index = rand()%10;
	int bin2Index = rand()%10;
	int bin3Index = rand()%10;

	//Bin1
	int i;
	for(i = 0; i < bin1Index; i++){
		//grab the first bin1Index items from bin1 of parent1
		m_bin1[i] = p1->getBin1At(i);
	}
	for(i = bin1Index; i < 10; i++){
		//grab 10 - bin1Index items from bin1 of parent2
		m_bin1[i] = p2->getBin1At(i);
	}

	//Bin2
	for(i = 0; i < bin2Index; i++){
		//grab the first bin2Index items from bin2 of parent1
		m_bin2[i] = p1->getBin2At(i);
	}
	for(i = bin2Index; i < 10; i++){
		//grab 10 - bin2Index items from bin2 of parent2
		m_bin2[i] = p2->getBin2At(i);
	}

	//Bin3
	for(i = 0; i < bin3Index; i++){
		//grab the first bin3Index items from bin3 of parent1
		m_bin3[i] = p1->getBin3At(i);
		}
	for(i = bin3Index; i < 10; i++){
		//grab 10 - bin3Index items from bin3 of parent2
		m_bin3[i] = p2->getBin3At(i);
	}
}

int Puzzle2DNA::getBin1At(int i) const {
	return m_bin1[i];
}
int Puzzle2DNA::getBin2At(int i) const {
	return m_bin2[i];
}
int Puzzle2DNA::getBin3At(int i) const {
	return m_bin3[i];
}

void Puzzle2DNA::Mutate() {
	std::vector<int> copyValidDNA = m_validPieces;
	int numDups = 0;

	for (std::vector<int>::iterator it = m_bin1.begin(); it != m_bin1.end(); ++it) {
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

//score the bin2 by multiplying its values
int Puzzle2DNA::getBin1Val() const {
	int score = 1;

	for(int a: m_bin1){
		score *= a;
	}

	return score;
}

//score the bin1 by adding its values
int Puzzle2DNA::getBin2Val() const {
	int score = 0;
	for(int a: m_bin2){
		score += a;
	}
}

void Puzzle2DNA::swapValues(int validIndex1, int validIndex2){
	int val1 = m_validPieces[validIndex1];
	int val2 = m_validPieces[validIndex2];
	int temp;

	int i,j;
	for(i = 0; i < 30; i++){
		for(j = 0; j < 30; j++){

				//check if val1 is in Bin1
				if(i < 10){

					//check if val1 is in bin1 and val2 is in bin1
					if(j < 10 && m_bin1[i] == val1 && m_bin1[j] == val2){
						temp = m_bin1[i];
						m_bin1[i] = m_bin1[j];
						m_bin1[j] = temp;
						return;
					}
					//check if val1 is in bin1 and val2 is in bin2
					else if(j < 20 && m_bin1[i] == val1 && m_bin2[j-10] == val2){
						temp = m_bin1[i];
						m_bin1[i] = m_bin2[j-10];
						m_bin2[j-10] = temp;
						return;

					}
					//check if val1 is in bin1 and val2 is in bin3
					else if(j < 30 && m_bin1[i] == val1 && m_bin3[j-20] == val2){
						temp = m_bin1[i];
						m_bin1[i] = m_bin3[j-20];
						m_bin3[j-20] = temp;
						return;
					}

				}
				//check if val1 is in bin 2
				else if(i < 20){
					//check if val1 is in bin2 and val2 is in bin1
					if(j < 10 && m_bin2[i - 10] == val1 && m_bin1[j] == val2){
						temp = m_bin2[i - 10];
						m_bin2[i - 10] = m_bin1[j];
						m_bin1[j] = temp;
						return;
					}

					//check if val1 is in bin2 and val2 is in bin2
					else if(j < 20 && m_bin2[i - 10] == val1 && m_bin2[j-10] == val2){
						temp = m_bin2[i - 10];
						m_bin2[i - 10] = m_bin2[j-10];
						m_bin2[j-10] = temp;
						return;
					}

					//check if val1 is in bin2 and val2 is in bin3
					else if(j < 30 && m_bin2[i - 10] == val1 && m_bin3[j-20] == val2){
						temp = m_bin2[i - 10];
						m_bin2[i - 10] = m_bin3[j-20];
						m_bin3[j-20] = temp;
						return;
					}
				}

				else if(i < 30){
					//check if val1 is in bin3 and val2 is in bin1
					if(j < 10 && m_bin3[i - 20] == val1 && m_bin1[j] == val2){
						temp = m_bin3[i - 20];
						m_bin3[i - 20] = m_bin1[j];
						m_bin1[j] = temp;
						return;
					}

					//check if val1 is in bin3 and val2 is in bin2
					else if(j < 20 && m_bin3[i - 20] == val1 && m_bin2[j-10] == val2){
						temp = m_bin3[i - 20];
						m_bin3[i - 20] = m_bin2[j-10];
						m_bin2[j-10] = temp;
						return;
					}

					//check if val1 is in bin3 and val2 is in bin3
					else if(j < 30 && m_bin3[i - 20] == val1 && m_bin3[j-20] == val2){
						temp = m_bin3[i - 20];
						m_bin3[i - 20] = m_bin3[j-20];
						m_bin3[j-20] = temp;
						return;
					}
				}

			}//end inner for
		}//end outer for

	}//end swapValues
bool Puzzle2DNA::binAtIndexFull(int index){
	if(index > 20){
		index = index - 20;
		return (m_bin1[index] != 0);
	}
	else if(index > 10){
		index = index - 10;
		return (m_bin2[index] != 0);
	}
	else
		return (m_bin3[index] !=0);
}

bool Puzzle2DNA::isValid(){

	int i, j;
	bool isValid = true;
	bool isMarked[30];

	if(!m_invalidPieces.empty()){
		m_invalidPieces.erase(m_invalidPieces.begin(), m_invalidPieces.end());
	}


	for(i = 0; i < 30; i++)
	{
		isMarked[i] = false;

		for(j = 0; j < 30; j++){

			if(!isMarked[i] && j < 10){
				if(m_bin1[j] == m_validPieces[i]){
					isMarked[i] = true;
				}

			}
			else if(!isMarked[i] && j < 20){
				if(m_bin2[j - 10] == m_validPieces[i]){
					isMarked[i] = true;
				}

			}
			else if(!isMarked[i] && j < 30){
				if(m_bin3[j - 20] == m_validPieces[i]){
					isMarked[i] = true;
				}
			}

		}

		if(isMarked[i] == false){
			isValid = false;
			m_invalidPieces.push_back(m_validPieces[i]);
		}
	}
	return isValid;
}

std::string Puzzle2DNA::ToString() {
	std::stringstream s;
	s << "bin 1:" << std::endl;
	for ( auto p : m_bin1) {
		s << p << "  ";
	}

	s << "bin 2:" << std::endl;
	for ( auto p : m_bin2) {
		s << p << "  ";
	}

	s << "bin 3:" << std::endl;
	for ( auto p : m_bin3) {
		s << p << "  ";
	}
	return s.str();
}
