#include "RRQueue.h"

RRQueue::RRQueue(int timeQuantum)
	: m_timeQuantum(timeQuantum), m_timeLeft(timeQuantum) { }

bool RRQueue::Update(int runTime)
{
	//static int timeLeft = m_timeQuantum; //determines whether or not CPU bursts are finished within the assigned time quantum.

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
		if (m_timeLeft != 0 || m_pItrNode != m_pFront)
			pCurrentProcess->DisplayProgress();

		//If process is not first in ready queue, increment its wait time.
		if (pCurrentProcess != nullptr && m_pItrNode != m_pFront)
		{
			pCurrentProcess->Wait();
		}
		else if (m_timeLeft != 0) //If process is first in ready queue, burst, and decrement time it has left to complete CPU burst.
		{
			--m_timeLeft;
			pCurrentProcess->Burst(runTime);
		}
		else if (m_timeLeft == 0)
		{
			m_timeLeft = m_timeQuantum;
			returnValue = true;
		}
	}
	else //If CPU burst is complete, notify update loop (in Main.cpp) that the current process will be transferred to I/O list.
	{
		m_timeLeft = m_timeQuantum;
		returnValue = true;
	}

	//Iterator will move to the next available process in the ready queue to update in the next iteration.
	IncrementIteratorNode();

	return returnValue;
}

void RRQueue::ResetTimeLeft()
{
	m_timeLeft = m_timeQuantum;
	//m_bWasInterrupted = true;
}
