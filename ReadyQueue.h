#pragma once
#include "Process.h"
#include <iostream>

#include <queue>

class ReadyQueue
{
private:
	struct Node
	{
		Process m_process;
		Node* Next;

		Node(Process&& process)
			: m_process(std::move(process)), Next(nullptr) { }
	};

private:
	Node* m_pBack;
	Node* m_pFront;
	Node* m_pItrNode;

	int m_size;

	bool iterationComplete;

private:
	void IncrementIteratorNode();
	void ResetIteratorNode();

public:
	ReadyQueue();
	ReadyQueue(const ReadyQueue& other);

	bool CompletedIteration();
	bool IsEmpty() const;
	bool Update();
	Process& Remove();
	void Add(Process&& process);
	void Print() const;

	void Sort();

	/*void Update()
	{
		for (int i = 0; i < queue.size(); ++i)
		{
			queue.front().Burst();
		}
	}*/

};