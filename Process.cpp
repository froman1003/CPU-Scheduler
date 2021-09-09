#include "Process.h"
#include <iostream>

unsigned int Process::counter = 0;

Process::Process(std::vector<int>& bursts)
	: m_bursts(bursts), m_currentBurst(m_bursts.begin())
{
	m_id = ++counter;
	m_bInitialBurst = true;
	m_name = new char[3];
	snprintf(m_name, 3, "P%d", m_id);

	m_bIsFinished = false;
	m_bDowngraded = false;

	m_turnaroundTime = 0;
	m_waitingTime = 0;
	m_responseTime = 0;
	m_burstIndex = 0;

	//std::cout << "Process #" << m_id << ": Constructed!" << std::endl;
}

Process::Process(Process& other)
	: m_bursts(other.m_bursts)
{
	m_id = other.m_id;
	m_bInitialBurst = other.m_bInitialBurst;

	m_name = new char[3];
	snprintf(m_name, 3, "P%d", m_id);

	m_bIsFinished = other.m_bIsFinished;
	m_bDowngraded = other.m_bDowngraded;

	m_currentBurst = m_bursts.begin();
	std::advance(m_currentBurst, std::distance(other.m_bursts.begin(), other.m_currentBurst));

	m_turnaroundTime = other.m_turnaroundTime;
	m_waitingTime = other.m_waitingTime;
	m_responseTime = other.m_responseTime;
	m_burstIndex = other.m_burstIndex;

	//std::cout << "Process #" << m_id << ": Copy Constructor has been called!" << std::endl;
}

Process::Process(Process&& other)
	: m_bursts(other.m_bursts)
{
	m_id = other.m_id;
	m_bInitialBurst = other.m_bInitialBurst;

	m_name = new char[3];
	snprintf(m_name, 3, "P%d", m_id);

	m_bIsFinished = other.m_bIsFinished;
	m_bDowngraded = other.m_bDowngraded;

	m_currentBurst = m_bursts.begin();
	std::advance(m_currentBurst, std::distance(other.m_bursts.begin(), other.m_currentBurst));

	m_turnaroundTime = other.m_turnaroundTime;
	m_waitingTime = other.m_waitingTime;
	m_responseTime = other.m_responseTime;
	m_burstIndex = other.m_burstIndex;

	other.m_turnaroundTime = 0;
	other.m_waitingTime = 0;
	other.m_responseTime = 0;

	//std::cout << "Process #" << m_id << ": has been moved!" << std::endl;
}

Process& Process::operator=(Process&& other)
{
	if (this != &other)
	{
		m_id = other.m_id;
		m_bInitialBurst = other.m_bInitialBurst;
		snprintf(m_name, 3, "P%d", m_id);
		m_bursts = other.m_bursts;
		m_currentBurst = m_bursts.begin();
		std::advance(m_currentBurst, std::distance(other.m_bursts.begin(), other.m_currentBurst));

		m_turnaroundTime = other.m_turnaroundTime;
		m_waitingTime = other.m_waitingTime;
		m_responseTime = other.m_responseTime;
		m_burstIndex = other.m_burstIndex;

		other.m_turnaroundTime = 0;
		other.m_waitingTime = 0;
		other.m_responseTime = 0;
	}

	return *this;
}

Process::~Process()
{
	delete m_name;
}

bool Process::IsBurstFinished() const
{
	return (*m_currentBurst) == 0;
}

bool Process::IsDowngraded() const
{
	return m_bDowngraded; 
}

//Has process completed all of its CPU and I/O bursts.
bool Process::IsProcessFinished() const
{
	return m_bIsFinished;
}

const char* Process::GetName() const
{
	return m_name;
}

int Process::GetBurst() const
{
	return *m_currentBurst;
}

int Process::GetTurnaroundTime() const
{
	return m_turnaroundTime;
}

//Increment the index of the burst to run. Called when current burst is finished.
void Process::NextBurst()
{
	if (m_currentBurst + 1 == m_bursts.end())
	{
		m_bIsFinished = true;
		return;
	}

	++m_currentBurst;
	++m_burstIndex;
}

void Process::Burst(unsigned int runTime)
{
	if (m_bInitialBurst)
	{
		m_responseTime = runTime;
		m_bInitialBurst = false;
	}

	--(*m_currentBurst);
	++m_turnaroundTime;
}

//FORMAT: [Process name (ex. P1) - Turnaround Time = CPU Burst Time + Waiting Time + I/O Time,
//Waiting Time = Time waiting to execute CPU Burst,
//Respone Time = Time of first CPU Burst]
void Process::DisplayFinalResults() const
{
	if (m_bIsFinished)
		printf("%s - Ttr: %d, Tw: %d, Tr: %d\n", m_name, m_turnaroundTime, m_waitingTime, m_responseTime);
	else
		printf("ERROR: Process is not finished! Cannot display final results!");
}

//FORMAT: [Process name (ex. P1): Process' current burst value (ex. 3)]
void Process::DisplayProgress() const
{
	printf("[%s: %d] ", m_name, *m_currentBurst);
}

void Process::SetDowngraded(bool condition)
{
	m_bDowngraded = condition;
}

void Process::Wait()
{
	++m_waitingTime;
	++m_turnaroundTime;
}