#pragma once
#include <vector>

class Process
{

	//VARIABLES
private:
	static unsigned int counter;
	unsigned int m_id;
	bool m_bInitialBurst;
	bool m_bIsFinished;
	char* m_name;

	unsigned int m_turnaroundTime;
	unsigned int m_waitingTime;
	unsigned int m_responseTime;
	unsigned int m_burstIndex;

	std::vector<int> m_bursts;
	std::vector<int>::iterator m_currentBurst;

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

	const char* GetName() const;

	int GetBurst() const;
	int GetTurnaroundTime() const;

	void Burst(unsigned int runTime);
	void DisplayProgress() const;
	void DisplayFinalResults() const;
	void NextBurst();
	void Wait();
};

