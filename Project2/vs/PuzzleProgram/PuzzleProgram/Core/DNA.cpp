#include "DNA.h"

DNA::DNA()
	: m_parent1(nullptr),
	m_parent2(nullptr)
{
}

DNA::DNA(const DNA& in_source1, const DNA& in_source2)
	: m_parent1(&in_source1),
	m_parent2(&in_source2)
{
}

DNA::~DNA()
{
}