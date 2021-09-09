#include<iostream>

#include "ReadyQueue.h"

ReadyQueue::ReadyQueue()
	: m_pBack(nullptr), m_pFront(nullptr), m_pItrNode(nullptr)
{
	m_size = 0;
	m_bIterationComplete = false;
}

//ReadyQueue::ReadyQueue(const ReadyQueue& other)
//	: m_pBack(nullptr), m_pFront(nullptr), m_pItrNode(nullptr)
//{
//	std::cout << "Deep Copy" << std::endl;
//
//	m_size = other.m_size;
//	m_bIterationComplete = other.m_bIterationComplete;
//
//	Node* pItrNode = other.m_pFront;
//
//	while (pItrNode)
//	{
//		Add(pItrNode->m_process);
//		pItrNode = pItrNode->Next;
//	}
//
//	m_pItrNode = new Node(std::move(other.m_pItrNode->m_process));
//}

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

//void ReadyQueue::Add(Process& process)
//{
//	if (m_pFront == nullptr)
//	{
//		m_pFront = new Node(process);
//		m_pBack = m_pFront;
//		m_pItrNode = m_pFront;
//	}
//	else
//	{
//		m_pBack->Next = new Node(process);
//		m_pBack = m_pBack->Next;
//	}
//
//	++m_size;
//}

//Notify update loop (in Main.cpp) if ready queue has finished iterating.
bool ReadyQueue::CompletedIteration()
{
	if (!m_bIterationComplete)
	{
		return m_bIterationComplete;
	}

	bool temp = m_bIterationComplete;
	m_bIterationComplete = false;
	return temp;
}

bool ReadyQueue::IsEmpty() const
{
	return m_size == 0;
}


//Update the current process in the ready queue
bool ReadyQueue::Update(int runTime)
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
			pCurrentProcess->Burst(runTime);
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
	m_bIterationComplete = true;
}

Process& ReadyQueue::Remove()
{
	if (m_pFront == nullptr)
	{
		std::cout << "ERROR: queue is empty!" << std::endl;
	}

	//Before removing current process from ready queue, its burst index is incremented to run I/O burst
	Process* pOrgProcess = &(m_pFront->m_process);

	if (pOrgProcess->IsBurstFinished() == true)
	{
		pOrgProcess->NextBurst();
	}
	else
	{
		pOrgProcess->SetDowngraded(true);
	}

	//Front of ready queue is empty - the ready queue has removed the current process
	m_pFront = m_pFront->Next;

	//Decrement size of ready queue, and check if ready queue is now empty.
	if (--m_size == 0)
		ResetIteratorNode();

	return *pOrgProcess;
}

//This sorts the ready queue/singly-linked list for Shortest Job First (SJF) scheduling - this is not what the program does at the moment
void ReadyQueue::Sort()
{
	if (m_size == 1)
		return;

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