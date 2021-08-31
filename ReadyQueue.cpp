#include<iostream>

#include "ReadyQueue.h"

ReadyQueue::ReadyQueue()
	: m_pBack(nullptr), m_pFront(nullptr), m_pItrNode(nullptr)
{
	m_size = 0;
	iterationComplete = false;
}

ReadyQueue::ReadyQueue(const ReadyQueue& other)
{
	std::cout << "Deep Copy" << std::endl;

	m_size = other.m_size;
	iterationComplete = other.iterationComplete;

	Node* pItrNode = other.m_pFront;

	while (pItrNode)
	{
		Add(std::move(pItrNode->m_process));
		pItrNode = pItrNode->Next;
	}

	m_pItrNode = new Node(std::move(other.m_pItrNode->m_process));
}

void ReadyQueue::Add(Process&& process)
{
	if (m_pFront == nullptr)
	{
		m_pFront = new Node(std::move(process));
		m_pBack = m_pFront;
		m_pItrNode = m_pFront;
	}
	else
	{
		m_pBack->Next = new Node(std::move(process));
		m_pBack = m_pBack->Next;
	}

	++m_size;
}

bool ReadyQueue::CompletedIteration()
{
	if (!iterationComplete)
	{
		return iterationComplete;
	}

	bool temp = iterationComplete;
	iterationComplete = false;
	return temp;
}

bool ReadyQueue::IsEmpty() const
{
	return m_size == 0;
}

bool ReadyQueue::Update()
{
	if (m_pFront == nullptr)
	{
		std::cout << "WARNING: No processes are ready!" << std::endl;
		return false;
	}

	Process* pCurrentProcess = &m_pItrNode->m_process;
	bool returnValue = false;

	/*if (pCurrentProcess->GetID() == 1 && pCurrentProcess->IsBurstFinished())
	{
		std::cout << "DEBUGGIN' TIME!" << std::endl;
	}*/

	if (pCurrentProcess->IsBurstFinished() == false)
	{
		printf("P%d ", pCurrentProcess->GetID());

		if (pCurrentProcess != nullptr && m_pItrNode != m_pFront)
		{
			pCurrentProcess->Wait();
		}
		else
		{
			pCurrentProcess->Burst();
		}
	}
	else
	{
		returnValue = true;
	}

	//if (m_size > 1)
	IncrementIteratorNode();

	return returnValue;
}

void ReadyQueue::Print() const
{
	std::cout << "Ready Queue: ";

	Node* pItrNode = m_pFront;

	while (pItrNode != nullptr)
	{

		pItrNode = pItrNode->Next;
	}

	std::cout << "\n";
}

void ReadyQueue::IncrementIteratorNode()
{
	if (m_pItrNode->Next == nullptr)
	{
		ResetIteratorNode();
		return;
	}

	m_pItrNode = m_pItrNode->Next;
}

void ReadyQueue::ResetIteratorNode()
{
	if (m_pItrNode != m_pFront && m_pFront != nullptr)
	{
		m_pItrNode = m_pFront;
	}
	else if (m_pFront == nullptr)
	{
		m_pItrNode = nullptr;
	}

	iterationComplete = true;
}

Process& ReadyQueue::Remove()
{
	if (m_pFront == nullptr)
	{
		std::cout << "ERROR: queue is empty!" << std::endl;
	}

	if (--m_size == 0)
		iterationComplete = true;

	Process* pCurrentProcess = &(m_pFront->m_process);

	pCurrentProcess->NextBurst();

	Process& pOrgFront = m_pFront->m_process;
	m_pFront = m_pFront->Next;
	return (pOrgFront);
}

void ReadyQueue::Sort()
{
	Node* pHead = m_pFront;

	while (pHead->Next != nullptr)
	{
		Node* pItrNode = pHead->Next;
		int headBurst = pHead->m_process.GetBurst();


		while (pItrNode != nullptr)
		{
			int itrBurst = pItrNode->m_process.GetBurst();

			if (headBurst > itrBurst)
			{
				Process tempProcess = pHead->m_process;
				pHead->m_process = std::move(pItrNode->m_process);
				pItrNode->m_process = std::move(tempProcess);
				headBurst = pHead->m_process.GetBurst();
			}

			pItrNode = pItrNode->Next;
		}

		pHead = pHead->Next;
	}
}