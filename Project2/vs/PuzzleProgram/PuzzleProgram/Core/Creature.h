#pragma once

class DNA;

class Creature
{
public:
	Creature(const Creature& in_parent1, const Creature& in_parent2);
	virtual ~Creature();

	float GetFitness() const { return m_fitness; }

protected:
	virtual DNA* CreateDNA() const = 0;
	virtual float CalculateFitness() = 0;
private:
	void Born(const Creature& in_parent1, const Creature& in_parent2);

	DNA* m_dna;
	unsigned int m_generation;
	float m_fitness;
};

