#pragma once
#include "ReadyQueue.h"

class MultiQueue
{
private:
	mutable unsigned int m_index;
	unsigned int m_priorityIndex;
	const int m_count;
	ReadyQueue** m_ppQueues;

private:
	bool AdjustIndex() const;

public:
	MultiQueue();

	bool CompletedIteration();

	//NOTE TO SELF: if one queue is empty, but processes will become available in said queue in the future, run subsequent
	//queue(s) in the meantime.

	bool IsEmpty() const;

	bool IsInOrder();

	bool Update(int runTime);
	void UpdateLowerPriorityQueues();

	int Index() const;

	Process& Remove();

	void Add(Process&& process);
	void AddToNextQueue(Process&& process);

	
};