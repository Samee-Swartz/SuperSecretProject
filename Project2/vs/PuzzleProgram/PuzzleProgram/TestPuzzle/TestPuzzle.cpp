#include "stdafx.h"
#include "TestPuzzle.h"


TestPuzzle::TestPuzzle()
{
}


TestPuzzle::~TestPuzzle()
{
}

void TestPuzzle::Setup(const std::string& in_fileName, unsigned& out_populationSize, unsigned& out_workerCount)
{
	out_populationSize = 1000;
	out_workerCount = 1;
}

Creature* TestPuzzle::CreateCreature() const
{
}