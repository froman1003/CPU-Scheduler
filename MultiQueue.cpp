#include "MultiQueue.h"
#include "RRQueue.h"

MultiQueue::MultiQueue()
	: m_index(0), m_priorityIndex(0), m_count(3)
{
	m_ppQueues = new ReadyQueue* [m_count];
	m_ppQueues[0] = new RRQueue(5);
	m_ppQueues[1] = new RRQueue(10);
	m_ppQueues[2] = new ReadyQueue();
}

bool MultiQueue::CompletedIteration()
{
	/*bool isCompleted = m_ppQueues[m_index]->CompletedIteration();

	if (isCompleted)
	{
		isCompleted = !AdjustIndex();
	}

	return isCompleted;*/

	return m_ppQueues[m_index]->CompletedIteration();
}

bool MultiQueue::IsEmpty() const
{
	bool isEmpty = m_ppQueues[m_index]->IsEmpty();

	if (isEmpty)
	{
		isEmpty = !AdjustIndex();
	}

	return isEmpty;
}

bool MultiQueue::AdjustIndex() const
{
	/*if (m_ppQueues[m_priorityIndex]->IsEmpty() == false)
		return;*/

	for (unsigned int i = m_index + 1; i < m_count; ++i)
	{
		if (m_ppQueues[i]->IsEmpty() == false)
		{
			m_ppQueues[m_index]->SetInterruption();
			m_index = i;
			return true;
		}
	}

	return false;
}

bool MultiQueue::Update(int runTime)
{
	bool returnValue = false;
	returnValue = m_ppQueues[m_index]->Update(runTime);

	return returnValue;
}

void MultiQueue::UpdateLowerPriorityQueues()
{
	for (unsigned int i = m_priorityIndex + 1; i < m_count; ++i)
	{
		if (m_index == i)
		{
			continue;
		}

		ReadyQueue& queue = *m_ppQueues[i];

		if (queue.IsEmpty() == false)
		{
			queue.UpdateWaitTimes();
		}
	}
}

int MultiQueue::Index() const
{
	return m_index;
}

void MultiQueue::Add(Process&& process)
{
	m_ppQueues[m_priorityIndex]->Add(std::move(process));

	if (m_index != m_priorityIndex)
	{
		m_ppQueues[m_index]->SetInterruption();
		m_index = m_priorityIndex;
	}
}

void MultiQueue::AddToNextQueue(Process&& process)
{
	if (m_index + 1 < m_count)
	{
		m_ppQueues[m_index + 1]->Add(std::move(process));
	}
}

Process& MultiQueue::Remove()
{
	//static int processesLeft = 8; //Number of processes in current queue.

	Process& pOrgFront = m_ppQueues[m_index]->Remove();

	//If CPU Burst was not completed within the time quantum, it does not belong in the current queue anymore.
	if (pOrgFront.IsDowngraded() == true || pOrgFront.IsProcessFinished())
	{
		//pOrgFront.SetDowngraded(false);
		//--processesLeft;
		//++m_queuesAvailable;

		//If no more processes will become available in the current queue, move to the next queue.
		if (m_ppQueues[m_index]->IsQueueFinished() == true)
		{
			//processesLeft = 8;
			++m_index;
			++m_priorityIndex;
			SetIterationComplete(m_ppQueues[m_index], true); //Ensures I/O will run before running the next queue. 
		}
	}

	return pOrgFront;
}

void SetIterationComplete(ReadyQueue* pQueue, bool condition)
{
	pQueue->m_bIterationComplete = true;
}