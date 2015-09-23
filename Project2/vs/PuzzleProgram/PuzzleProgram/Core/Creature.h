#pragma once

#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>

class DNA;

class Creature
{
public:
	Creature();
	virtual ~Creature();

	void SetParents(const Creature& in_parent1, const Creature& in_parent2);

	float GetFitness() const { return m_fitness; }
	float GetScore() const { return m_score; }
	const DNA& GetDNA() const { return *m_dna; }

	unsigned int GetGeneration() const { return m_generation; }

	const Creature* GetParent1() const { return m_parent1; }
	const Creature* GetParent2() const { return m_parent2; }

	void Lock();
	bool TryLock();
	void Unlock();

protected:
	friend class Puzzle;

	virtual DNA* CreateDNA() const = 0;
	virtual float CalculateFitness() = 0;
	virtual float CalculateScore() = 0;
private:
	void OnInit();

	const Creature* m_parent1;
	const Creature* m_parent2;

	DNA* m_dna;
	unsigned int m_generation;
	float m_fitness;
	float m_score;

	bool m_isFree;

	boost::mutex m_mutex;
};

