#include "TestDNA.h"
#include <random>

void TestDNA::Generate()
{
	for (int i = 0; i < 10; i++)
	{
		m_data[i] = rand();
	}
}

void TestDNA::Splice()
{
	int i;
	for (i = 0; i < 5; i++)
	{
		m_data[i] = static_cast<const TestDNA*>(GetParent1())->m_data[i];
	}

	for (; i < 10; i++)
	{
		m_data[i] = static_cast<const TestDNA*>(GetParent2())->m_data[i];
	}
}

void TestDNA::Mutate()
{
}