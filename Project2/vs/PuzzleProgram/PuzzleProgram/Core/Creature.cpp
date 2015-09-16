#include "stdafx.h"
#include "DNA.h"
#include "Creature.h"
#include <algorithm>

Creature::Creature(const Creature& in_parent1, const Creature& in_parent2)
{
	Born(in_parent1, in_parent2);
}

Creature::~Creature()
{
}

void Creature::Born(const Creature& in_parent1, const Creature& in_parent2)
{
	m_generation = std::max(in_parent1.m_generation, in_parent2.m_generation);

	m_dna = CreateDNA();
	m_dna->Splice();
	m_dna->Mutate();
}
