#include <iostream>
#include <time.h>

#include "Puzzle.h"
#include "../Puzzle3/Puzzle3.h"

#define POPULATION_SIZE 1000
#define WORKERS_SIZE 64
#include "../Puzzle2/Puzzle2.h"

void usage() {
	std::cout << "This program will run a genetic algorithm on one of "
		<< "three puzzles. To use run: " << std::endl;
	std::cout << "./ga PUZZLE FILE SECONDS" << std::endl;
	std::cout << "\twhere PUZZLE is either 1, 2, or 3" << std::endl;
	std::cout << "\tFILE is the name of the file containing puzzle information" << std::endl;
	std::cout << "\tSECONDS are the number of seconds to run for" << std::endl;
}


int main(int argc, char** argv) {
	if (argc != 4) {
		usage();
		return 1;
	}

	if (atoi(argv[3]) == 0) {
		usage();
		return 1;
	}

	srand(39458);

	Puzzle* p;

	switch(atoi(argv[1])) {
		case 0:
			usage();
			return 1;
		case 1:
			// p = new Puzzle1();
			break;
		case 2:
			p = new Puzzle2();
			break;
		case 3:
			p = new Puzzle3();
			break;
		default:
			usage();
			return 1;
	}
	p->Run(argv[2], atoi(argv[3]));
	system("pause");
	std::cout << "TODO: ADD THE FUNCTIONS FOR THE BEST SHIT HERE" << std::endl;
	// Creature best = p->GetBestCreature();
	// std::cout << "Ran " << p->GetFinalGeneration() << " generations." << std::endl;
	// std::cout << "The best offspring was:" << std::endl << best << std::endl;
}

/*
current best offspring found
current best score
current best generation #
final generation #
 */
