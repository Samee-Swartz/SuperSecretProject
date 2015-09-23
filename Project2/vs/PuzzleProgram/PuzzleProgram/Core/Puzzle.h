#pragma once

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>
#include "Creature.h"
#include "GenerationWorker.h"
#include <stack>
#include <queue>

class Puzzle
{
public:

	explicit Puzzle();

	virtual ~Puzzle();

	void Run(const std::string& in_fileName, unsigned int in_runtime);

protected:
	virtual void Setup(const std::string& in_fileName, unsigned int& out_populationSize, unsigned int& out_workerCount) = 0;

	virtual Creature* CreateCreature() const = 0;

private:
	void CreatePopulation(unsigned int in_populationSize);

	void CreateWorkers(unsigned int in_workerSize);

	void SelectNextParents(unsigned int& out_parent1, unsigned int& out_parent2);

	void AddToNext(Creature& in_creature);

	void SwapPopulations();

	void StopWorkers();

	void WorkerGenerator();

	void BreedingThread();

private:
	unsigned int m_populationSize;
	std::vector<Creature*>* m_polulation;
	std::vector<Creature*>* m_nextPopulation;

	std::queue<std::pair<Creature*, Creature*>> m_pairs;

	std::vector<boost::thread*> m_workers;

	boost::condition_variable m_workerTrigger;

	boost::mutex m_populationLock;
	boost::mutex m_nextPopulationLock;
	boost::mutex m_pairsAccess;
};

