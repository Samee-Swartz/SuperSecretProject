#include "Puzzle2.h"

/*
holds vector Creatures - current population
current best offspring found
current best generation #
final generation #
Selection rule - how to choose parents
runs crossover
*/

void Puzzle2::Setup(std::string in_file, unsigned in& out_populationSize, unsigned int& out_workerCount){
	std::ifstream givenWorld(in_fileName.c_str());
	std::vector<int> validPieces
	
	if(!givenWorld){
		std::cout << "Given input file is invalid." << std::endl;
	}
	
	std::string line; 
	
	int temp;
	while(std::getline(givenWorld, line)){
		temp = atoi(line);
		
		if(temp > 10 || temp < -10){
			std::cout << "Invalid value for Puzzle2 validPieces:" << temp << std::endl;
		}
		
		validPieces.push_back(temp);
	}
}