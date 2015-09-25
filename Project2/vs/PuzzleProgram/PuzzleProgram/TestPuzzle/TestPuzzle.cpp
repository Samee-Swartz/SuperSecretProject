#include "stdafx.h"
#include "TestPuzzle.h"
#include "TestCreature.h"


TestPuzzle::TestPuzzle()
{
}


TestPuzzle::~TestPuzzle()
{
}

void TestPuzzle::Setup(const std::string& in_fileName, unsigned& out_populationSize, unsigned& out_workerCount)
{
	out_populationSize = 20;
	out_workerCount = 10;
}

Creature* TestPuzzle::CreateCreature() const
{
	return new TestCreature();
}