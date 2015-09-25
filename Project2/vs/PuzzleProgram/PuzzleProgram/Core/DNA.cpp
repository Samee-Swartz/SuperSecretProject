#include "DNA.h"

DNA::DNA()
	: m_parent1(nullptr),
	m_parent2(nullptr)
{
}

DNA::~DNA()
{
}

std::string DNA::ToString() const
{
	return "";
}

void DNA::InternalSplice(const DNA& in_parent1, const DNA& in_parent2)
{
	m_parent1 = &in_parent1;
	m_parent2 = &in_parent2;

	Splice();
}