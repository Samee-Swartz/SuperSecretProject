#include "DNA.h"
#include "Creature.h"
#include <algorithm>

Creature::Creature()
	: m_parent1(nullptr),
	m_parent2(nullptr)
{
	static unsigned int globalIdCounter = 0;
	globalIdCounter++;
	m_id = globalIdCounter;
}

Creature::~Creature()
{
	delete m_dna;
}

void Creature::SetParents(const Creature& in_parent1, const Creature& in_parent2)
{
	m_parent1 = &in_parent1;
	m_parent2 = &in_parent2;
}

std::string Creature::ToString() const
{
	return  "Generation: " + std::to_string(m_generation) + "\n"
		+ "Score: " + std::to_string(m_score) + "\n"
		+ "Fitness: " + std::to_string(m_fitness) + "\n"
		+ "DNA: " + m_dna->ToString() + "\n";
}

void Creature::OnInit()
{
	m_dna = CreateDNA();

	if(m_parent1 == nullptr || m_parent2 == nullptr)
	{
		m_generation = 0;
		m_dna->Generate();
	}
	else
	{
		m_generation = m_parent1->m_generation;
		m_dna->InternalSplice(m_parent1->GetDNA(), m_parent2->GetDNA());
		m_dna->Mutate();
	}

	m_fitness = CalculateFitness();
	m_score = CalculateScore();
}
