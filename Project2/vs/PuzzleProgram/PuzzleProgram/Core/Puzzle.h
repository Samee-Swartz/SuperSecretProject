#pragma once

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include "Creature.h"
#include "GenerationWorker.h"
#include <stack>
#include <queue>
#include <random>

class Puzzle
{
public:

	explicit Puzzle();

	virtual ~Puzzle();

	void Run(const std::string& in_fileName, unsigned int in_runtime, const std::string& in_saveFileName = "Data.csv");

protected:
	virtual void Setup(const std::string& in_fileName, unsigned int& out_populationSize, unsigned int& out_workerCount) = 0;

	virtual Creature* CreateCreature() const = 0;

private:
	void CreatePopulation(unsigned int in_populationSize);

	void CreateWorkers(unsigned int in_workerSize);

	void SelectNextParents(unsigned int& out_parent1, unsigned int& out_parent2);

	void AddToNext(Creature& in_creature);

	void Cull();
	void Elite();
	void SwapPopulations();

	void StopWorkers();

	void WorkerGenerator();
	Creature* CreateBaby(const Creature* in_parent1, const Creature* in_parent2) const;
	void BreedingThread();

	void DestroyerThread();

private:
	unsigned int m_generation;

	unsigned int m_populationSize;
	std::vector<Creature*>* m_polulation;
	std::vector<Creature*>* m_nextPopulation;
	std::vector<Creature*> m_murderPopulation;

	std::queue<std::pair<Creature*, Creature*>> m_pairs;
	volatile unsigned int m_jobCount;

	std::vector<boost::thread*> m_workers;

	boost::condition_variable m_workerTrigger;
	boost::condition_variable m_murderTrigger;

	boost::recursive_mutex m_populationLock;
	boost::recursive_mutex m_nextPopulationLock;
	boost::mutex m_pairsAccess;
	boost::mutex m_murderAccess;
	boost::recursive_mutex m_jobCountAccess;

	std::default_random_engine m_randEngine;

	std::string m_saveFileName;
};

