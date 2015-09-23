#include "Puzzle1.h"

void Puzzle3::Setup(std::string in_file, unsigned int& out_populationSize, unsig$
 
	std::ifstream givenFile(in_file.c_str());

	if(!givenFile){ //there's a problem
		std::cout << "Input file is invalid" << std::end;
		return;
	}

	std::string line;

	std::string target;
	std::getline(givenFile, target);
	m_target = atoi(target);

	while(std::getline(givenFile, line)){
		int temp = atoi(line);

		if(temp == 0){
			//there's a problem
		}

		m_validDNA.push_back(temp);
	}
}

