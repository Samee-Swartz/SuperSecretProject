#pragma once

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>
#include "Creature.h"
#include "GenerationWorker.h"
#include <stack>

class Puzzle
{
public:

	explicit Puzzle(unsigned int in_populationSize);

	virtual ~Puzzle();

	void Run(unsigned int in_seconds, unsigned int in_workers);

protected:
	virtual Creature* CreateCreature() const = 0;
	virtual Creature* CreateCreature(const Creature& in_parent1, const Creature& in_parent2) const = 0;

private:
	void CreatePopulation(unsigned int in_populationSize);

	void CreateWorkers(unsigned int in_workerSize);

	void SelectNextParents(unsigned int& out_parent1, unsigned int& out_parent2);

	void InsertCreature(Creature& in_creature);

	void StartWorkers();

	void StopWorkers();

	void WorkerGenerator();

	void BreedingThread();

private:
	std::vector<Creature*> m_polulation;

	std::stack<std::pair<Creature*, Creature*>*> m_pairs;

	std::vector<boost::thread*> m_workers;

	boost::condition_variable m_workerTrigger;

	boost::mutex m_populationLock;
	boost::mutex m_pairsAccess;
};

