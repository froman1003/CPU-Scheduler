#pragma once
#include "ReadyQueue.h"

class MultiQueue
{
private:
	int m_index;
	const int m_count;
	ReadyQueue** m_ppQueues;

public:
	MultiQueue();

	bool CompletedIteration();

	//NOTE TO SELF: if one queue is empty, but processes will become available in said queue in the future, run subsequent
	//queue(s) in the meantime.

	bool IsEmpty() const;

	bool Update(int runTime);

	int Index() const;

	Process& Remove();

	void Add(Process&& process, unsigned int index);
};