#include "Puzzle.h"
#include <boost/chrono/duration.hpp>
#include <filesystem>

Puzzle::Puzzle()
	: m_polulation(new std::vector<Creature*>),
	m_nextPopulation(new std::vector<Creature*>)
{
}

Puzzle::~Puzzle()
{
	delete m_polulation;
	delete m_nextPopulation;
}

void Puzzle::Run(const std::string& in_fileName, unsigned int in_runtime)
{
	unsigned int workerCount;

	Setup(in_fileName, m_populationSize, workerCount);

	CreateWorkers(workerCount);

	boost::chrono::seconds runTimeSeconds(in_runtime);

	boost::thread mainThread = boost::thread(boost::bind(&Puzzle::WorkerGenerator, this));

	boost::this_thread::sleep_for(runTimeSeconds);

	mainThread.interrupt();
	mainThread.join();
}

void Puzzle::CreatePopulation(unsigned int in_populationSize)
{
	for (unsigned int i = 0; i < in_populationSize; i++)
	{
		Creature* newCreature = CreateCreature();
		newCreature->OnInit();
		m_polulation->push_back(newCreature);
	}
}

void Puzzle::CreateWorkers(unsigned int in_workerSize)
{
	for (unsigned int i = 0; i < in_workerSize; i++)
	{
		boost::thread* worker = new boost::thread(boost::bind(&Puzzle::BreedingThread, this));
		m_workers.push_back(worker);
	}
}

void Puzzle::SelectNextParents(unsigned int& out_parent1, unsigned int& out_parent2)
{
	out_parent1 = 0;
	out_parent2 = rand() % 5 % (m_polulation->size() - 1) + 1;
}

void Puzzle::AddToNext(Creature& in_creature)
{
	boost::unique_lock<boost::mutex> lock(m_nextPopulationLock);

	float fitness = in_creature.GetFitness();

	int i;
	for (i = 0; i < m_nextPopulation->size(); i++)
	{
		Creature* creature = (*m_nextPopulation)[i];
		
		if (creature->GetFitness() < fitness)
			break;
	}

	m_nextPopulation->insert(m_nextPopulation->begin() + i, &in_creature);
}

void Puzzle::SwapPopulations()
{
	boost::unique_lock<boost::mutex> populationLock(m_populationLock);
	boost::unique_lock<boost::mutex> nextLock(m_nextPopulationLock);

	auto& temp = m_polulation;
	m_polulation = m_nextPopulation;
	m_nextPopulation = m_polulation;
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
	CreatePopulation(m_populationSize);

	while(true)
	{
		try
		{
			//Check to see if the main thread has signled us to stop
			boost::this_thread::interruption_point();
		}
		catch(boost::exception&)
		{
			//If we have been signled to stop then shutdown all worker threads and wait
			StopWorkers();
			throw;
		}

		int lastSize = m_polulation->size();

		while (m_polulation->size() != 0)
		{
			Creature* c1;
			Creature* c2;

			{
				boost::unique_lock<boost::mutex> lock(m_populationLock);

				unsigned int creature1Index = 0, creature2Index = 0;

				SelectNextParents(creature1Index, creature2Index);

				c1 = (*m_polulation)[creature1Index];
				c2 = (*m_polulation)[creature2Index];

				m_polulation->erase(m_polulation->begin() + creature1Index);
				if (creature1Index < creature2Index)
					creature2Index -= 1;
				m_polulation->erase(m_polulation->begin() + creature2Index);
			}


			{
				boost::unique_lock<boost::mutex> lock(m_pairsAccess);

				m_pairs.push(std::pair<Creature*, Creature*>(c1, c2));
			}

			m_workerTrigger.notify_one();
		}

		while(m_nextPopulation->size() != lastSize || m_pairs.size() != 0)
		{
			boost::this_thread::yield();
		}

		SwapPopulations();
	}
}

void Puzzle::BreedingThread()
{
	while (true)
	{
		boost::unique_lock<boost::mutex> lock(m_pairsAccess);

		while (m_pairs.size() == 0)
		{
			m_workerTrigger.wait(lock);
		}

		std::pair<Creature*, Creature*>& pair = m_pairs.front();
		m_pairs.pop();

		lock.release();

		Creature* const baby = CreateCreature();
		baby->SetParents(*pair.first, *pair.second);
		baby->OnInit();

		std::array<Creature*, 3> all;
		all[0] = pair.first;
		all[1] = pair.second;
		all[2] = baby;

		std::sort(all.begin(), all.end(), [](Creature* a, Creature* b)
		{
			return b->GetFitness() < a->GetFitness();
		});

		
		AddToNext(*all[0]);
		AddToNext(*all[1]);

		delete all[2];

		boost::this_thread::interruption_point();
	}
}