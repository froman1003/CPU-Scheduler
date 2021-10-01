#pragma once
#include "ReadyQueue.h"

class RRQueue : public ReadyQueue
{

private:
	unsigned int m_timeQuantum;
	unsigned int m_timeLeft;

public:
	RRQueue(int timeQuantum);

	virtual bool Update(int runTime) override;
	virtual void ResetTimeLeft() override;
	
};

