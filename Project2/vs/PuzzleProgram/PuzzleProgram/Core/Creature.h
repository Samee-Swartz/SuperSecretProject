#pragma once

#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>

class DNA;

class Creature
{
public:
	Creature();
	Creature(const Creature& in_parent1, const Creature& in_parent2);
	virtual ~Creature();

	float GetFitness() const { return m_fitness; }
	const DNA& GetDNA() const { return *m_dna; }

	void Lock();
	bool TryLock();
	void Unlock();

protected:
	virtual DNA* CreateDNA() const = 0;
	virtual float CalculateFitness() = 0;
private:
	void Born(const Creature& in_parent1, const Creature& in_parent2);

	DNA* m_dna;
	unsigned int m_generation;
	float m_fitness;

	bool m_isFree;

	boost::mutex m_mutex;
};

