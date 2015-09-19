#pragma once

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/thread.hpp>
#include "Creature.h"
#include "GenerationWorker.h"
#include <queue>
#include <stack>
#include <thread>

namespace std{
	class thread;
}

class GenerationWorker;

template<class C>
class Puzzle
{
public:

	explicit Puzzle(unsigned int in_populationSize)
	{
		CreatePopulation(in_populationSize);
	}

	virtual ~Puzzle()
	{
	}

	void Run(unsigned int in_seconds, unsigned int in_workers)
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

protected:
	virtual void SelectParents(int& out_parent1, int& out_parent2) const = 0;

private:
	void CreatePopulation(unsigned int in_populationSize)
	{
		for (unsigned int i = 0; i < in_populationSize; i++)
		{
			C* newCreature = new C();
			m_polulation.push_back(newCreature);
		}
	}

	void CreateWorkers(unsigned int in_workerSize)
	{
		for (unsigned int i = 0; i < in_workerSize; i++)
		{
			boost::thread* worker = new boost::thread(boost::bind(&WorkerThread, this));
			m_workers.push_back(worker);
		}
	}

	void StartWorkers()
	{
		for(auto& worker : m_workers)
		{
			worker->start_thread();
		}
	}

	void StopWorkers()
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

	

	void WorkerGenerator()
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
				throw e;
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

				SelectParents(creature1Index, creature2Index);

				c1 = m_polulation[creature1Index];
				c2 = m_polulation[creature2Index];

				m_polulation.erase(m_polulation.begin() + creature1Index);
				if (creature1Index < creature2Index)
					creature2Index -= 1;
				m_polulation.erase(m_polulation.begin() + creature2Index);
			}
			
			
			{
				boost::unique_lock<boost::mutex> lock(m_taskStackAccess);

				GenerationWorker* newWorker = new GenerationWorker(*c1, *c2);
				m_tasks.push(newWorker);
			}

			m_workerTrigger.notify_one();
		}
	}

	void WorkerThread()
	{
		while (true)
		{
			boost::unique_lock<boost::mutex> lock(m_taskStackAccess);

			while (m_tasks.size() == 0)
			{
				m_workerTrigger.wait(lock);
			}

			GenerationWorker* const nextWorker = m_tasks.top();
			m_tasks.pop();

			lock.release();

			nextWorker->Run();

			boost::this_thread::interruption_point();
		}
	}

private:
	std::vector<C> m_polulation;

	std::stack<GenerationWorker*> m_tasks;

	std::vector<boost::thread*> m_workers;

	boost::condition_variable m_workerTrigger;

	boost::mutex m_populationLock;
	boost::mutex m_taskStackAccess;
};

