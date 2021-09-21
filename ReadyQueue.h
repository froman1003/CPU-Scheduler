#pragma once
#include "Process.h"
#include <iostream>

#include <queue>

class ReadyQueue
{
protected:
	struct Node
	{
		Process m_process;
		Node* Next;

		Node(Process&& process)
			: m_process(std::move(process)), Next(nullptr) { }

		Node(Process& process)
			: m_process(process), Next(nullptr) { }
	};

	Node* m_pBack;
	Node* m_pFront;
	Node* m_pItrNode;

	int m_size;
	int m_processesLeft;

	bool m_bIterationComplete;
	bool m_bWasInterrupted;

protected:
	void IncrementIteratorNode();
	void ResetIteratorNode();
	friend void SetIterationComplete(ReadyQueue* pQueue, bool condition);

public:
	ReadyQueue();
	ReadyQueue(const ReadyQueue& other) = delete;

	bool CompletedIteration();
	bool IsEmpty() const;
	bool IsQueueFinished() const;
	virtual bool Update(int runTime);
	Process& Remove();
	void Add(Process& process) = delete;
	void Add(Process&& process);
	void Print() const;
	void SetInterruption();
	void Sort();
	
};