#pragma once
#include <Core/Puzzle.h>

class TestPuzzle : public Puzzle
{
public:
	TestPuzzle();
	virtual ~TestPuzzle();
protected:
	virtual void Setup(const std::string& in_fileName, unsigned& out_populationSize, unsigned& out_workerCount) override;
	virtual Creature* CreateCreature() const override;
};

