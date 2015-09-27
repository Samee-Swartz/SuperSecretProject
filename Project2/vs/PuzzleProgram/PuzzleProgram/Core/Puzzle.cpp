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

	boost::thread deleteThread = boost::thread(boost::bind(&Puzzle::DestroyerThread, this));
	boost::thread mainThread = boost::thread(boost::bind(&Puzzle::WorkerGenerator, this));

	boost::this_thread::sleep_for(runTimeSeconds);

	mainThread.interrupt();
	mainThread.join();

	deleteThread.interrupt();
	deleteThread.join();

	std::cout << "BEST CREATURE:" << std::endl << (*m_polulation)[0]->ToString();
}

void Puzzle::CreatePopulation(unsigned int in_populationSize)
{
	for (unsigned int i = 0; i < in_populationSize; i++)
	{
		Creature* newCreature = CreateCreature();
		newCreature->OnInit();
		newCreature->m_generation = 0;
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

	m_nextPopulation->insert(m_nextPopulation->begin(), &in_creature);
}

void Puzzle::Cull()
{
	boost::unique_lock<boost::recursive_mutex> populationLock(m_populationLock);

	int cullIndex = m_populationSize;;

	if(cullIndex < m_polulation->size())
	{
		boost::unique_lock<boost::mutex> murderLock(m_murderAccess);
		m_murderPopulation.clear();

		for (int i = cullIndex; i < m_polulation->size(); i++)
		{
			m_murderPopulation.push_back((*m_polulation)[i]);
		}
	}

	m_murderTrigger.notify_one();

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

	std::sort(m_nextPopulation->begin(), m_nextPopulation->end(), [](const Creature* const a, const Creature* const b)
	{
		return a->GetFitness() > b->GetFitness();
	});

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

	m_generation = 1;

	while(true)
	{
		std::cout << "Starting Generation " << m_generation << std::endl;
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
		m_generation++;
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
		baby->m_generation = m_generation;

		AddToNext(*baby);
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

void Puzzle::DestroyerThread()
{
	while(true)
	{
		try
		{
			boost::this_thread::interruption_point();
		}
		catch(boost::exception&)
		{
			return;
		}

		boost::unique_lock<boost::mutex> lock(m_murderAccess);

		while(m_murderPopulation.size() == 0)
		{
			m_murderTrigger.wait(lock);
		}

		for(Creature* c : m_murderPopulation)
		{
			delete c;
		}

		m_murderPopulation.clear();
	}
}