#pragma once

class Creature;

/*
Represents a creatures DNA
*/
class DNA
{
public:
	DNA(const DNA& in_source1, const DNA& in_source2);
	virtual ~DNA();

protected:
	friend class Creature;

	virtual void Splice() = 0;
	virtual void Mutate() = 0;

	const DNA& GetParent1() const { return m_parent1; }
	const DNA& GetParent2() const { return m_parent2; }

private:
	const DNA& m_parent1;
	const DNA& m_parent2;
};
