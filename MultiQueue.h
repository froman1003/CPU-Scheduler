#pragma once
#include "ReadyQueue.h"

class MultiQueue
{
private:
	const int m_count;
	mutable unsigned int m_index;
	unsigned int m_priorityIndex;

	ReadyQueue** m_ppQueues;

	mutable bool m_bAllQueuesAreEmpty;

private:
	bool AdjustIndex() const;
	void UpdateLowerPriorityQueues();
	void UpdateProcessCounts(unsigned int index);

public:
	MultiQueue();

	bool CompletedIteration();

	//NOTE TO SELF: if one queue is empty, but processes will become available in said queue in the future, run subsequent
	//queue(s) in the meantime.

	bool IsEmpty() const;

	bool IsInOrder();
	bool IsFinished() const;

	bool Update(int runTime);

	int Index() const;

	Process& Remove();

	void Add(Process&& process);
};