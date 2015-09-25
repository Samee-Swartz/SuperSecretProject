#include <string>
#include <fstream>
#include <time.h>
#include <iostream>
#include "Puzzle1DNA.h"
#include "Puzzle1.h"

#define WORKER_COUNT 10
#define POPULATION_SIZE 1000


void Puzzle1::Setup(const std::string& in_fileName, unsigned int& out_populationSize, unsigned int& out_workerCount){
	std::ifstream givenFile(in_fileName.c_str());

	if(!givenFile){
		std::cout << "Input file is invalid" << std::endl;
		return;
	}

	std::string line;
	std::string target;
	std::getline(givenFile, target);
	int targetI = atoi(target.c_str());
	std::vector<int> dna;

	while(std::getline(givenFile, line)){
		int temp = atoi(line.c_str());

		if(temp == 0){
			//there's a
			std::cout << line << " was interpreted as 0" << std::endl;
		}

		dna.push_back(temp);
	}

	Puzzle1DNA::SetValidPieces(dna, targetI);
	out_populationSize = POPULATION_SIZE;
	out_workerCount = WORKER_COUNT;
}

