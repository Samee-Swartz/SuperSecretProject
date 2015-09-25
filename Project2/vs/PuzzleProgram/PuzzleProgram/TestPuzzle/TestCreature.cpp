#include "stdafx.h"
#include "TestCreature.h"
#include "TestDNA.h"


TestCreature::TestCreature()
	: Creature()
{
}


TestCreature::~TestCreature()
{
}

DNA* TestCreature::CreateDNA() const
{
	return new TestDNA();
}

float TestCreature::CalculateFitness()
{
	int sum = 0;

	for (int i = 0; i < 10; i++)
	{
		const TestDNA& dna = static_cast<const TestDNA&>(GetDNA());
		sum += dna.GetData(i);
	}

	return static_cast<float>(abs(sum)) / 100.f;
}

float TestCreature::CalculateScore()
{
	return GetFitness();
}