#include "Puzzle2.h"
#include <fstream>
#include "Puzzle2DNA.h"
/*
holds vector Creatures - current population
current best offspring found
current best generation #
final generation #
Selection rule - how to choose parents
runs crossover
*/
#define WORKER_COUNT 8
#define POPULATION_SIZE 500

void Puzzle2::Setup(const std::string& in_fileName, unsigned int& out_populationSize, unsigned int& out_workerCount){
	std::ifstream givenWorld(in_fileName.c_str());
	std::vector<float> validPieces;

	if(!givenWorld){
		std::cout << "Given input file is invalid." << std::endl;
	}

	std::string line;

	float temp;
	while(std::getline(givenWorld, line)){
		temp = atof(line.c_str());

		if(temp > 10 || temp < -10){
			std::cout << "Invalid value for Puzzle2 validPieces:" << temp << std::endl;
		}

		validPieces.push_back(temp);
	}

	Puzzle2DNA::SetValidPieces(validPieces);
	out_populationSize = POPULATION_SIZE;
	out_workerCount = WORKER_COUNT;
}

Creature* Puzzle2::CreateCreature() const
{
	return new Puzzle2Creature();
}