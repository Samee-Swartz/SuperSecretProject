#include "Puzzle.h"
#include <boost/chrono/duration.hpp>

Puzzle::Puzzle()
	: m_polulation(new std::vector<Creature*>),
	m_nextPopulation(new std::vector<Creature*>),
	m_jobCount(0)
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

	std::cout << "BEST CREATURE:" << std::endl << (*m_polulation)[0]->ToString();
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
	boost::unique_lock<boost::recursive_mutex> lock(m_nextPopulationLock);

	float fitness = in_creature.GetFitness();

	int i;
	for (i = 0; i < m_nextPopulation->size(); i++)
	{
		Creature* creature = (*m_nextPopulation)[i];
		
		if (creature->GetFitness() < fitness)
			break;
	}

	in_creature.m_generation++;

	m_nextPopulation->insert(m_nextPopulation->begin() + i, &in_creature);
}

void Puzzle::Cull()
{
	boost::unique_lock<boost::recursive_mutex> populationLock(m_populationLock);

	int count = m_polulation->size() - m_populationSize;

	int i = 0;
	for (auto itr = m_polulation->rbegin(); itr != m_polulation->rend() && i < count; ++itr, ++i)
	{
		delete *itr;
	}

	m_polulation->resize(m_populationSize);
}

void Puzzle::SwapPopulations()
{
	boost::unique_lock<boost::recursive_mutex> populationLock(m_populationLock);
	boost::unique_lock<boost::recursive_mutex> nextLock(m_nextPopulationLock);

	for(Creature* creature : (*m_polulation))
	{
		AddToNext(*creature);
	}

	m_polulation->clear();

	auto temp = m_polulation;
	m_polulation = m_nextPopulation;
	m_nextPopulation = temp;
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

	unsigned int generation = 0;

	while(true)
	{
		std::cout << "Starting Generation " << generation << std::endl;
		try
		{
			//Check to see if the main thread has signled us to stop
			boost::this_thread::interruption_point();
		}
		catch(boost::exception&)
		{
			//If we have been signled to stop then shutdown all worker threads and wait
			StopWorkers();
			return;
		}

		int lastSize = m_polulation->size();

		while (m_polulation->size() >= 2)
		{
			Creature* c1;
			Creature* c2;

			{
				boost::unique_lock<boost::recursive_mutex> lock(m_populationLock);

				unsigned int creature1Index = 0, creature2Index = 0;

				SelectNextParents(creature1Index, creature2Index);

				c1 = (*m_polulation)[creature1Index];
				c2 = (*m_polulation)[creature2Index];

				if (c1 == c2)
					break;

				m_polulation->erase(m_polulation->begin() + creature1Index);
				if (creature1Index < creature2Index)
					creature2Index -= 1;
				m_polulation->erase(m_polulation->begin() + creature2Index);
			}


			{
				boost::unique_lock<boost::mutex> pairLock(m_pairsAccess);
				boost::unique_lock<boost::recursive_mutex> jobCountLock(m_jobCountAccess);

				m_pairs.push(std::pair<Creature*, Creature*>(c1, c2));
				m_jobCount++;
			}

			m_workerTrigger.notify_one();
		}

		while(m_jobCount != 0)
		{
			boost::this_thread::yield();
		}

		SwapPopulations();
		Cull();
		generation++;
	}
}

void Puzzle::BreedingThread()
{
	while (true)
	{
		boost::unique_lock<boost::mutex> lock(m_pairsAccess);

		while(m_pairs.size() == 0)
			m_workerTrigger.wait(lock);

		std::pair<Creature*, Creature*> pair = m_pairs.front();
		m_pairs.pop();

		lock.unlock();

		Creature* const baby = CreateCreature();
		baby->SetParents(*pair.first, *pair.second);
		baby->OnInit();

		//AddToNext(*baby);
		AddToNext(*pair.first);
		AddToNext(*pair.second);
		{
			boost::unique_lock<boost::recursive_mutex> jobCountLock(m_jobCountAccess);
			m_jobCount--;
		}

		try
		{
			boost::this_thread::interruption_point();
		}
		catch(boost::exception&)
		{
			return;
		}
	}
}