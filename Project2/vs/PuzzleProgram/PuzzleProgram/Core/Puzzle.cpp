#include "Puzzle.h"
#include <boost/chrono/duration.hpp>

Puzzle::Puzzle(unsigned int in_populationSize)
{
}

Puzzle::~Puzzle()
{
}

void Puzzle::Run(unsigned int in_seconds, unsigned int in_workers)
{
	CreateWorkers(in_workers);
	StartWorkers();

	boost::chrono::seconds runTimeSeconds(in_seconds);

	boost::thread mainThread = boost::thread(boost::bind(&WorkerGenerator, this));
	mainThread.start_thread();

	boost::this_thread::sleep_for(runTimeSeconds);

	mainThread.interrupt();
	mainThread.join();
}

void Puzzle::CreatePopulation(unsigned in_populationSize)
{
	for (unsigned int i = 0; i < in_populationSize; i++)
	{
		Creature* newCreature = CreateCreature();
		m_polulation.push_back(newCreature);
	}
}

void Puzzle::CreateWorkers(unsigned in_workerSize)
{
	for (unsigned int i = 0; i < in_workerSize; i++)
	{
		boost::thread* worker = new boost::thread(boost::bind(&WorkerThread, this));
		m_workers.push_back(worker);
	}
}

void Puzzle::StartWorkers()
{
	for(auto& worker : m_workers)
	{
		worker->start_thread();
	}
}

void Puzzle::StopWorkers()
{
	for(auto& worker : m_workers)
	{
		worker->interrupt();
	}

	for (auto& worker : m_workers)
	{
		worker->join();
	}
}