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

bool MultiQueue::AdjustIndex() const
{
	for (unsigned int i = m_priorityIndex; i < m_count; ++i)
	{
		if (m_ppQueues[i]->IsEmpty() == false)
		{
			if (m_index != i)
			{
				m_ppQueues[m_index]->SetInterruption();
				m_index = i;
			}
			
			return true;
		}
	}

	return false;
}

bool MultiQueue::CompletedIteration()
{
	return m_ppQueues[m_index]->CompletedIteration();
}

bool MultiQueue::IsEmpty() const
{
	/*if (m_index != m_priorityIndex)
	{
		AdjustIndex();
	}*/

	bool condition = m_ppQueues[m_index]->IsEmpty();

	if (condition)
	{
		condition = !AdjustIndex();
	}

	return condition;
	//return m_ppQueues[m_index]->IsEmpty();
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

void MultiQueue::Add(Process&& process)
{
	m_ppQueues[m_priorityIndex]->Add(std::move(process));

	m_index = m_index != m_priorityIndex ? m_priorityIndex : m_index;
	/*if (m_index != m_priorityIndex)
	{
		AdjustIndex();
	}*/
}

void MultiQueue::AddToNextQueue(Process&& process)
{
	int index = m_index + 1;

	if (index < m_count)
	{
		m_ppQueues[index]->Add(std::move(process));
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