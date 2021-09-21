#pragma once
#include "ReadyQueue.h"

class RRQueue : public ReadyQueue
{

private:
	unsigned int m_timeQuantum;

public:
	RRQueue(int timeQuantum);

	virtual bool Update(int runTime) override;
	
};

