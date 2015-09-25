#pragma once
#include <Core/Creature.h>


class TestCreature : public Creature
{
public:
	TestCreature();
	virtual ~TestCreature();

protected:
	virtual DNA* CreateDNA() const override;
	virtual float CalculateFitness() override;
	virtual float CalculateScore() override;
};

