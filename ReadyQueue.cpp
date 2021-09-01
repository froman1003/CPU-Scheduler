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

//Notify update loop (in Main.cpp) if ready queue has finished iterating.
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


//Update the current process in the ready queue
bool ReadyQueue::Update()
{
	if (m_pFront == nullptr)
	{
		std::cout << "WARNING: No processes are ready!" << std::endl;
		return false;
	}

	Process* pCurrentProcess = &m_pItrNode->m_process;
	bool returnValue = false;

	//Check if current process' CPU burst is complete
	if (pCurrentProcess->IsBurstFinished() == false)
	{
		pCurrentProcess->DisplayProgress();

		//If process is not first in ready queue, increment its wait time.
		if (pCurrentProcess != nullptr && m_pItrNode != m_pFront)
		{
			pCurrentProcess->Wait();
		}
		else //If process is first in ready queue, burst.
		{
			pCurrentProcess->Burst();
		}
	}
	else //If CPU burst is complete, notify update loop (in Main.cpp) that the current process will be transferred to I/O list.
	{
		returnValue = true;
	}

	//Iterator will move to the next available process in the ready queue to update in the next iteration.
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

//Increment iterator node to the next available process in ready queue. Reset iterator node if reached end of ready queue.
void ReadyQueue::IncrementIteratorNode()
{
	if (m_pItrNode->Next == nullptr)
	{
		ResetIteratorNode();
		return;
	}

	m_pItrNode = m_pItrNode->Next;
}

//Move iterator back to the front of the ready queue.
void ReadyQueue::ResetIteratorNode()
{
	if (m_pItrNode != m_pFront && m_pFront != nullptr)
	{
		m_pItrNode = m_pFront;
	}
	else if (m_pFront == nullptr) //If ready queue is empty, iterator node should point to nothing.
	{
		m_pItrNode = nullptr;
	}

	//Iteration of ready queue is complete.
	iterationComplete = true;
}

Process& ReadyQueue::Remove()
{
	if (m_pFront == nullptr)
	{
		std::cout << "ERROR: queue is empty!" << std::endl;
	}

	//Before removing current process from ready queue, its burst index is incremented to run I/O burst
	Process* pCurrentProcess = &(m_pFront->m_process);
	pCurrentProcess->NextBurst();
	Process& pOrgFront = *pCurrentProcess;

	//Front of ready queue is empty - the ready queue has removed the current process
	m_pFront = m_pFront->Next;

	//Decrement size of ready queue, and check if ready queue is now empty.
	if (--m_size == 0)
		ResetIteratorNode();

	return (pOrgFront);
}

//This sorts the ready queue/singly-linked list for Shortest Job First (SJF) scheduling - this is not what the program does at the moment
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