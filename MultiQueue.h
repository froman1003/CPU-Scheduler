#pragma once
#include "ReadyQueue.h"

class MultiQueue
{
private:
	int m_index;
	ReadyQueue m_queues[3];

public:
	MultiQueue();

	bool CompletedIteration();
	bool IsEmpty() const;
	bool Update(int runTime);

	int Index() const;

	Process& Remove();

	void Add(Process&& process, unsigned int index);
};