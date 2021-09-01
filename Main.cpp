#include "ReadyQueue.h"

#include <vector>

void FCFS(ReadyQueue& readyQueue)
{
	unsigned int counter = 0;
	std::vector<Process*> ioList;
	std::vector<Process*>::iterator ioIterator;
	ioList.reserve(8);

	do
	{
		printf("RUNTIME: %d\n", counter);
		printf("Ready Queue: ");

		//Update all processes in ready queue
		while (readyQueue.CompletedIteration() == false && readyQueue.IsEmpty() == false)
		{
			bool receivedProcess = readyQueue.Update();

			//If current process has completed its CPU burst, retrieve it.
			if (receivedProcess)
			{
				Process& returnedProcess = readyQueue.Remove();

				//Transfer retrieved/returned process to I/O list if not finished.
				if (returnedProcess.IsProcessFinished() == false)
				{
					printf("(%s has left ready queue!) ", returnedProcess.GetName());
					ioList.emplace_back(std::move(&returnedProcess));
				}
				else
				{
					printf("(%s IS FINISHED!) ", returnedProcess.GetName());
				}
			}
		}

		std::cout << "\n";

		printf("I/O Queue: ");

		//Update all processes in I/O list.
		for (ioIterator = ioList.begin(); ioIterator != ioList.end();)
		{
			Process& process = **ioIterator;

			//Run current I/O burst and increment iterator to run next I/O burst.
			if (process.IsBurstFinished() == false)
			{
				process.DisplayProgress();
				process.Burst();
				++ioIterator;
			}
			else //Transfer current process to ready queue when its I/O burst is completed.
			{
				process.NextBurst();
				printf("(%s has left I/O queue!) ", process.GetName());
				readyQueue.Add(std::move(process));
				ioIterator = ioList.erase(ioIterator);
			}
		}

		std::cout << "\n\n";

		++counter;

	} while (readyQueue.IsEmpty() == false || ioList.empty() == false);
}

void FCFS(std::vector<Process> processes);

int main()
{
	ReadyQueue readyQueue;
	std::vector<int> bursts;
	bursts.reserve(20);

	//Creates all processes and inserts them into ready queue
	bursts.assign({ 5, 27, 3, 31, 5, 43, 4, 18, 6, 22, 4, 26, 3, 24, 4 });
	readyQueue.Add(std::move(Process(bursts)));

	bursts.assign({ 4, 48, 5, 44, 7, 42, 12, 37, 9, 76, 4, 41, 9, 31, 7, 43, 8 });
	readyQueue.Add(std::move(Process(bursts)));

	bursts.assign({ 8, 33, 12, 41, 18, 65, 14, 21, 4, 61, 15, 18, 14, 26, 5, 31, 6 });
	readyQueue.Add(std::move(Process(bursts)));

	bursts.assign({ 3, 35, 4, 41, 5, 45, 3, 51, 4, 61, 5, 54, 6, 82, 5, 77, 3 });
	readyQueue.Add(std::move(Process(bursts)));

	bursts.assign({ 16, 24, 17, 21, 5, 36, 16, 26, 7, 31, 13, 28, 11, 21, 6, 13, 3, 11, 4 });
	readyQueue.Add(std::move(Process(bursts)));

	bursts.assign({ 11, 22, 4, 8, 5, 10, 6, 12, 7, 14, 9, 18, 12, 24, 15, 30, 8 });
	readyQueue.Add(std::move(Process(bursts)));

	bursts.assign({ 14, 46, 17, 41, 11, 42, 15, 21, 4, 32, 7, 19, 16, 33, 10 });
	readyQueue.Add(std::move(Process(bursts)));

	bursts.assign({ 4, 14, 5, 33, 6, 51, 14, 73, 16, 87, 6 });
	readyQueue.Add(std::move(Process(bursts)));

	//THIS IS TO TEST SORTING ALGORITHM FOR SJF SCHEDULING.
	//readyQueue.Sort();

	//Runs first-come-first-serve scheduling algorithm
	FCFS(readyQueue);
}



void DisplayInfo(ReadyQueue& readyQueue, std::vector<Process*>& ioList)
{
	
}