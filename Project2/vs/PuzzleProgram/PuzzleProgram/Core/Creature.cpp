#include "DNA.h"
#include "Creature.h"
#include <algorithm>

Creature::Creature()
{
}

Creature::~Creature()
{
	
}

void Creature::SetParents(const Creature& in_parent1, const Creature& in_parent2)
{
	m_parent1 = &in_parent1;
	m_parent2 = &in_parent2;
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

void Creature::OnInit()
{
	boost::unique_lock<boost::mutex> lock(m_mutex);

	m_dna = CreateDNA();

	if(m_parent1 == nullptr || m_parent2 == nullptr)
	{
		m_generation = -1;
		m_dna->Generate();
	}
	else
	{
		m_generation = m_parent1->m_generation;
		m_dna->InternalSplice(m_parent1->GetDNA(), m_parent2->GetDNA());
		m_dna->Mutate();
	}

	CalculateFitness();
}
