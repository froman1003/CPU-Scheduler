#include "MultiQueue.h"
#include "RRQueue.h"

MultiQueue::MultiQueue()
	: m_index(0), m_numOfQueues(3), m_count(3)
{
	m_ppQueues = new ReadyQueue* [m_count];
	m_ppQueues[0] = new RRQueue(5);
	m_ppQueues[1] = new RRQueue(10);
	m_ppQueues[2] = new ReadyQueue();
}

bool MultiQueue::CompletedIteration()
{
	return m_ppQueues[m_index]->CompletedIteration();
}

bool MultiQueue::IsEmpty() const
{
	return m_ppQueues[m_index]->IsEmpty();
}

bool MultiQueue::Update(int runTime)
{
	bool returnValue = false;
	returnValue = m_ppQueues[m_index]->Update(runTime);

	return returnValue;
}

int MultiQueue::Index() const
{
	return m_index;
}

void MultiQueue::Add(Process&& process, unsigned int index)
{
	m_ppQueues[index]->Add(std::move(process));
}

Process& MultiQueue::Remove()
{
	static int processesLeft = 8; //Number of processes in current queue.

	Process& pOrgFront = m_ppQueues[m_index]->Remove();

	//If CPU Burst was not completed within the time quantum, it does not belong in the current queue anymore.
	if (m_numOfQueues > 1 && pOrgFront.IsDowngraded() == true)
	{
		//pOrgFront.SetDowngraded(false);
		--processesLeft;

		//If no more processes will become available in the current queue, move to the next queue.
		if (processesLeft == 0)
		{
			processesLeft = 8;
			++m_index;
			--m_numOfQueues;
		}
	}

	return pOrgFront;
}