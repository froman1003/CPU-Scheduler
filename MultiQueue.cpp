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
	bool isCompleted = m_ppQueues[m_index]->CompletedIteration();
	
	if (isCompleted)
	{
		UpdateLowerPriorityQueues();
	}

	return isCompleted;
}

bool MultiQueue::IsEmpty() const
{
	if (m_bAllQueuesAreEmpty)
		return true;

	bool isEmpty = m_ppQueues[m_index]->IsEmpty();

	if (isEmpty)
	{
		isEmpty = !AdjustIndex();
	}

	return isEmpty;
}

bool MultiQueue::IsFinished() const
{
	if (m_bAllQueuesAreEmpty)
		return true;

	if (m_priorityIndex < m_count - 1)
		return false;

	bool condition = m_ppQueues[m_count - 1]->IsQueueFinished();
	m_bAllQueuesAreEmpty = condition;

	return condition;
}

bool MultiQueue::AdjustIndex() const
{
	for (unsigned int i = m_priorityIndex; i < m_count; ++i)
	{
		if (m_index == i)
			continue;

		if (m_ppQueues[i]->IsEmpty() == false)
		{
			m_index = i;
			return true;
		}
	}

	m_bAllQueuesAreEmpty = true;
	return false;
}

bool MultiQueue::Update(int runTime)
{
	return m_ppQueues[m_index]->Update(runTime);
}

void MultiQueue::UpdateLowerPriorityQueues()
{
	for (unsigned int i = m_index + 1; i < m_count; ++i)
	{
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
	unsigned int index = process.GetQueueID();
	m_ppQueues[index]->Add(std::move(process));

	if (index < m_index)
	{
		//m_ppQueues[m_index]->ResetTimeLeft();
		m_index = index;
	}

	m_bAllQueuesAreEmpty = false;
}

Process& MultiQueue::Remove()
{
	Process& pOrgFront = m_ppQueues[m_index]->Remove();

	if (pOrgFront.IsProcessFinished() == true)
	{
		UpdateProcessCounts(pOrgFront.GetQueueID());
	}

	//If no more processes will become available in the current queue, move to the next queue.
	if (m_ppQueues[m_index]->IsQueueFinished() == true && m_priorityIndex < m_count - 1)// && IsFinished() == false)
	{
		++m_priorityIndex;

		if (m_index < m_priorityIndex)
		{
			m_index = m_priorityIndex;
			SetIterationComplete(m_ppQueues[m_index]);
		}
	}
			
	return pOrgFront;
}

//If a process has finished, all queues with a lower priority have to be "notified" that there will be one less process
//that could potentially join them.
void MultiQueue::UpdateProcessCounts(unsigned int index)
{
	for (int i = index + 1; i < m_count; ++i)
	{
		ReadyQueue* queue = m_ppQueues[i];
		ReduceProcessCount(queue);
	}
}

void ReduceProcessCount(ReadyQueue* pQueue)
{
	--pQueue->m_processesLeft;
}

void SetIterationComplete(ReadyQueue* pQueue)
{
	pQueue->m_bIterationComplete = true;
}
