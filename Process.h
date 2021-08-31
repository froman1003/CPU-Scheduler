#pragma once
#include <vector>

class Process
{

	//VARIABLES
private:
	static unsigned int counter;
	unsigned int m_id;

	unsigned int m_turnaroundTime;
	unsigned int m_waitingTime;
	unsigned int m_responseTime;
	unsigned int m_burstIndex;

	std::vector<int> m_bursts;
	std::vector<int>::iterator m_currentBurst;

	bool m_bIsFinished;

	//METHODS
public:
	Process() = delete;
	Process(std::vector<int>& bursts);
	Process(Process& other);
	Process(Process&& other);
	~Process();

	Process& operator=(Process&& other);

	bool IsBurstFinished() const;
	bool IsProcessFinished() const;

	int GetBurst() const;
	int GetID() const;
	int GetTurnaroundTime() const;

	void Burst();
	void NextBurst();
	void Wait();


};

