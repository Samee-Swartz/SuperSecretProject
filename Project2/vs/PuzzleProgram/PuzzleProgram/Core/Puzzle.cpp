#include "Puzzle.h"
#include <boost/chrono/duration.hpp>
#include <filesystem>

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
		boost::thread* worker = new boost::thread(boost::bind(&BreedingThread, this));
		m_workers.push_back(worker);
	}
}

void Puzzle::SelectNextParents(unsigned& out_parent1, unsigned& out_parent2)
{
	out_parent1 = 0;
	out_parent2 = rand() % 5 % (m_polulation.size() - 1) + 1;
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

void Puzzle::WorkerGenerator()
{
	while(true)
	{
		try
		{
			//Check to see if the main thread has signled us to stop
			boost::this_thread::interruption_point();
		}
		catch(boost::exception& e)
		{
			//If we have been signled to stop then shutdown all worker threads and wait
			StopWorkers();
			throw;
		}

		//If the population is empty then we should just wait
		if (m_polulation.size() == 0)
		{
			boost::this_thread::yield();
			continue;
		}

		Creature* c1;
		Creature* c2;

		{
			boost::unique_lock<boost::mutex> lock(m_populationLock);

			int creature1Index, creature2Index;

			

			c1 = m_polulation[creature1Index];
			c2 = m_polulation[creature2Index];

			m_polulation.erase(m_polulation.begin() + creature1Index);
			if (creature1Index < creature2Index)
				creature2Index -= 1;
			m_polulation.erase(m_polulation.begin() + creature2Index);
		}
			
			
		{
			boost::unique_lock<boost::mutex> lock(m_pairsAccess);

			
		}

		m_workerTrigger.notify_one();
	}
}

void Puzzle::BreedingThread()
{
	while (true)
	{
		boost::unique_lock<boost::mutex> lock(m_pairsAccess);

		while (m_pairs.size() < 2)
		{
			m_workerTrigger.wait(lock);
		}

		std::pair<Creature*, Creature*>* pair = m_pairs.top();
		m_pairs.pop();

		lock.release();

		Creature* const baby = CreateCreature(*pair->first, *pair->second);

		std::array<Creature*, 3> all;
		all[0] = pair->first;
		all[1] = pair->second;
		all[2] = baby;

		std::sort(all.begin(), all.end(), [](Creature* a, Creature* b)
		{
			return b->GetFitness() < a->GetFitness();
		});

		lock.lock();



		boost::this_thread::interruption_point();
	}
}