#include "Process.h"
#include <iostream>

unsigned int Process::counter = 0;

Process::Process(std::vector<int>& bursts)
	: m_bursts(bursts), m_currentBurst(m_bursts.begin())
{
	m_id = ++counter;

	m_bIsFinished = false;

	m_turnaroundTime = 0;
	m_waitingTime = 0;
	m_responseTime = 0;
	m_burstIndex = 0;

	std::cout << "Process #" << m_id << ": Constructed!" << std::endl;
}

Process::Process(Process& other)
	: m_bursts(other.m_bursts)
{
	m_id = other.m_id;
	m_bIsFinished = other.m_bIsFinished;
	m_currentBurst = m_bursts.begin();
	std::advance(m_currentBurst, std::distance(other.m_bursts.begin(), other.m_currentBurst));

	m_turnaroundTime = other.m_turnaroundTime;
	m_waitingTime = other.m_waitingTime;
	m_responseTime = other.m_responseTime;
	m_burstIndex = other.m_burstIndex;

	std::cout << "Process #" << m_id << ": Copy Constructor has been called!" << std::endl;
}

Process::Process(Process&& other)
	: m_bursts(other.m_bursts)
{
	m_id = other.m_id;
	m_bIsFinished = other.m_bIsFinished;
	m_currentBurst = m_bursts.begin();
	std::advance(m_currentBurst, std::distance(other.m_bursts.begin(), other.m_currentBurst));

	m_turnaroundTime = other.m_turnaroundTime;
	m_waitingTime = other.m_waitingTime;
	m_responseTime = other.m_responseTime;
	m_burstIndex = other.m_burstIndex;

	other.m_turnaroundTime = 0;
	other.m_waitingTime = 0;
	other.m_responseTime = 0;

	std::cout << "Process #" << m_id << ": has been moved!" << std::endl;
}

Process& Process::operator=(Process&& other)
{
	if (this != &other)
	{
		m_id = other.m_id;
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
}

bool Process::IsBurstFinished() const
{
	return (*m_currentBurst) == 0;
}

bool Process::IsProcessFinished() const
{
	return m_bIsFinished;
}

int Process::GetBurst() const
{
	return *m_currentBurst;
}

int Process::GetID() const
{
	return m_id;
}

int Process::GetTurnaroundTime() const
{
	return m_turnaroundTime;
}

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

void Process::Burst()
{
	--(*m_currentBurst);
	++m_turnaroundTime;
}

void Process::Wait()
{
	++m_waitingTime;
	++m_turnaroundTime;
}