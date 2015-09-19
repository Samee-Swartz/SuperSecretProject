#pragma once
class Creature;

class GenerationWorker
{
public:
	GenerationWorker(const Creature& in_parent1, const Creature& in_parent2);
	~GenerationWorker();

	void Run();

private:
	
};

