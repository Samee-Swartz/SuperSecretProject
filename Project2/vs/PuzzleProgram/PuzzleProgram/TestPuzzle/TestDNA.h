#pragma once
#include <Core/DNA.h>

class TestDNA : public DNA
{
public:
	int GetData(int in_index) const { return m_data[in_index]; }
protected:
	virtual void Generate() override;
	virtual void Splice() override;
	virtual void Mutate() override;

private:
	int m_data[10];
};
