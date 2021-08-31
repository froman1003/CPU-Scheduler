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
		printf("Ready Queue (Line %d): ", ++counter);

		while (readyQueue.CompletedIteration() == false && readyQueue.IsEmpty() == false)
		{
			bool receivedProcess = readyQueue.Update();

			if (receivedProcess)
			{
				Process& returnedProcess = readyQueue.Remove();

				if (returnedProcess.IsProcessFinished() == false)
				{
					printf("(P%d has left ready queue!) ", returnedProcess.GetID());
					ioList.emplace_back(std::move(&returnedProcess));
				}
				else
				{
					printf("(P%d IS FINISHED!) ", returnedProcess.GetID());
				}
				/*Process removedProcess = std::move(readyQueue.Remove());
				ioList.push_back(std::move(removedProcess));*/
			}
		}

		std::cout << "\n";

		printf("I/O Queue (Line %d): ", counter);

		for (ioIterator = ioList.begin(); ioIterator != ioList.end();)
		{
			Process& process = **ioIterator;

			if (process.IsBurstFinished() == false)
			{
				printf("P%d ", process.GetID());
				process.Burst();
				++ioIterator;
			}
			else
			{
				process.NextBurst();
				printf("(P%d has left I/O queue!) ", process.GetID());
				readyQueue.Add(std::move(process));
				ioIterator = ioList.erase(ioIterator);
			}
		}

		std::cout << "\n";

	} while (readyQueue.IsEmpty() == false || ioList.empty() == false);
}

void FCFS(std::vector<Process> processes);

int main()
{
	ReadyQueue readyQueue;
	std::vector<int> bursts;
	bursts.reserve(20);

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

	FCFS(readyQueue);
}