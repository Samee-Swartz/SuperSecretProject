#pragma once

class Creature;

/*
Represents a creatures DNA
*/
class DNA
{
public:
	DNA();
	virtual ~DNA();

protected:
	friend class Creature;

	//Called when there are no parents, generates a random DNA
	virtual void Generate() = 0;
	//Called when there are parents, splices the parent DNA toghether
	virtual void Splice() = 0;
	//Called when there are parents, mutates the current dna
	virtual void Mutate() = 0;

	const DNA* GetParent1() const { return m_parent1; }
	const DNA* GetParent2() const { return m_parent2; }

private:
	void InternalSplice(const DNA& in_parent1, const DNA& in_parent2);

	const DNA* m_parent1;
	const DNA* m_parent2;
};

