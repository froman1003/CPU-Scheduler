#pragma once
#include "ReadyQueue.h"

class MultiQueue
{
private:
	int m_index;
	int m_numOfQueues;
	const int m_count;
	ReadyQueue** m_ppQueues;

public:
	MultiQueue();

	bool CompletedIteration();
	bool IsEmpty() const;
	bool Update(int runTime);

	int Index() const;

	Process& Remove();

	void Add(Process&& process, unsigned int index);
};