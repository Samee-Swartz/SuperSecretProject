#include "DNA.h"
#include "Creature.h"
#include <algorithm>

Creature::Creature()
{
}

Creature::Creature(const Creature& in_parent1, const Creature& in_parent2)
{
	Born(in_parent1, in_parent2);
}

Creature::~Creature()
{
	
}

void Creature::Lock()
{
	m_mutex.lock();
}

bool Creature::TryLock()
{
	return m_mutex.try_lock();
}

void Creature::Unlock()
{
	m_mutex.unlock();
}

void Creature::Born(const Creature& in_parent1, const Creature& in_parent2)
{
	boost::unique_lock<boost::mutex> lock(m_mutex);

	m_generation = std::max(in_parent1.m_generation, in_parent2.m_generation);

	m_dna = CreateDNA();
	m_dna->Splice();
	m_dna->Mutate();

	CalculateFitness();
}
