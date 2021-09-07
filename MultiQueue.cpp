#include "MultiQueue.h"

MultiQueue::MultiQueue()
	: m_index(0)
{

}

bool MultiQueue::CompletedIteration()
{
	return m_queues[m_index].CompletedIteration();
}

bool MultiQueue::IsEmpty() const
{
	return m_queues[m_index].IsEmpty();
}

bool MultiQueue::Update(int runTime)
{
	bool returnValue = false;
	returnValue = m_queues[m_index].Update(runTime);

	return returnValue;
}

int MultiQueue::Index() const
{
	return m_index;
}

void MultiQueue::Add(Process&& process, unsigned int index)
{
	m_queues[index].Add(std::move(process));
}

Process& MultiQueue::Remove()
{
	Process& pOrgFront = m_queues[m_index].Remove();

	if (m_queues[m_index].IsEmpty())
	{
		++m_index;
	}

	return pOrgFront;
}