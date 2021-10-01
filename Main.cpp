#include "ReadyQueue.h"
#include "MultiQueue.h"
#include <vector>

void DisplayResults(const std::vector<Process*>& finishedProcesses, const char* algName);

void FCFS(ReadyQueue& readyQueue)
{
	unsigned int runTime = 0;

	using ProcessList = std::vector<Process*>;
	ProcessList ioList;
	ProcessList finishedProcesses;
	ioList.reserve(8);

	do
	{
		printf("RUNTIME: %d\n", runTime);
		printf("Ready Queue: ");

		//Update all processes in ready queue
		while (readyQueue.CompletedIteration() == false && readyQueue.IsEmpty() == false)
		{
			bool receivedProcess = readyQueue.Update(runTime);

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
					finishedProcesses.emplace_back(&returnedProcess);
					printf("(%s IS FINISHED!) ", returnedProcess.GetName());
				}
			}
		}

		std::cout << "\n";

		printf("I/O Queue: ");

		//Update all processes in I/O list.
		for (ProcessList::iterator ioIterator = ioList.begin(); ioIterator != ioList.end();)
		{
			Process& process = **ioIterator;

			//Run current I/O burst and increment iterator to run next I/O burst.
			if (process.IsBurstFinished() == false)
			{
				process.DisplayProgress();
				process.Burst(runTime);

				if (process.IsBurstFinished() == true) //Transfer current process to ready queue when its I/O burst is completed.
				{
					process.NextBurst();
					printf("(%s has left I/O queue!) ", process.GetName());
					readyQueue.Add(std::move(process));
					ioIterator = ioList.erase(ioIterator);
				}
				else
				{
					++ioIterator;
				}
			}
		}

		std::cout << "\n\n";

		++runTime;

	} while (readyQueue.IsEmpty() == false || ioList.empty() == false);

	DisplayResults(finishedProcesses, "FCFS");
}

void SJF(ReadyQueue& readyQueue)
{
	unsigned int runTime = 0;
	bool sortReadyQueue = true;

	using ProcessList = std::vector<Process*>;
	ProcessList ioList;
	ProcessList finishedProcesses;
	ioList.reserve(8);
	

	do
	{
		if (sortReadyQueue)
		{
			readyQueue.Sort();
			sortReadyQueue = false;
		}

		printf("RUNTIME: %d\n", runTime);
		printf("Ready Queue: ");

		//Update all processes in ready queue
		while (readyQueue.CompletedIteration() == false && readyQueue.IsEmpty() == false)
		{
			bool receivedProcess = readyQueue.Update(runTime);

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
					finishedProcesses.emplace_back(&returnedProcess);
					printf("(%s IS FINISHED!) ", returnedProcess.GetName());
				}
			}
		}

		std::cout << "\n";

		printf("I/O Queue: ");

		//Update all processes in I/O list.
		for (ProcessList::iterator ioIterator = ioList.begin(); ioIterator != ioList.end();)
		{
			Process& process = **ioIterator;

			//Run current I/O burst and increment iterator to run next I/O burst.
			if (process.IsBurstFinished() == false)
			{
				process.DisplayProgress();
				process.Burst(runTime);

				if (process.IsBurstFinished() == true) //Transfer current process to ready queue when its I/O burst is completed.
				{
					process.NextBurst();
					printf("(%s has left I/O queue!) ", process.GetName());
					readyQueue.Add(std::move(process));
					ioIterator = ioList.erase(ioIterator);
					sortReadyQueue = true; //Ensures ready queue is sorted the next time it is updated.
				}
				else
				{
					++ioIterator;
				}
			}
		}

		std::cout << "\n\n";

		++runTime;

	} while (readyQueue.IsEmpty() == false || ioList.empty() == false);

	DisplayResults(finishedProcesses, "SJF");
}





//P6 SEEMS TO HAVE GONE BACK
void MLFQ(MultiQueue& mq)
{
	unsigned int runTime = 0;

	using ProcessList = std::vector<Process*>;
	ProcessList ioList;
	ProcessList finishedProcesses;
	ioList.reserve(8);

	do
	{
		int queueNum = mq.Index() + 1;
		printf("RUNTIME: %d\n", runTime);
		printf("Queue %d: ", queueNum);

		//Update all processes in ready queue
		while (mq.CompletedIteration() == false && mq.IsFinished() == false)
		{
			bool receivedProcess = mq.Update(runTime);

			//If current process has completed its CPU burst, retrieve it.
			if (receivedProcess)
			{
				Process& returnedProcess = mq.Remove();
				bool downgraded = returnedProcess.IsDowngraded();
				bool finished = returnedProcess.IsProcessFinished();

				//Transfer retrieved/returned process to I/O list if not finished.
				if (!downgraded && !finished)
				{
					printf("(%s has left Queue %d!) ", returnedProcess.GetName(), queueNum);
					ioList.emplace_back(std::move(&returnedProcess));
				}
				else if (downgraded && !finished)
				{
					printf("(%s has moved to Queue %d!) ", returnedProcess.GetName(), queueNum + 1);
					returnedProcess.SetDowngraded(false);
					//mq.Add(std::move(returnedProcess));
					mq.AddToNextQueue(std::move(returnedProcess));
				}
				else
				{
					printf("(%s IS FINISHED!) ", returnedProcess.GetName());
					finishedProcesses.emplace_back(&returnedProcess);
				}
			}
		}

		std::cout << "\n";
		printf("I/O Queue: ");

		//Update all processes in I/O list.
		for (ProcessList::iterator ioIterator = ioList.begin(); ioIterator != ioList.end();)
		{
			Process& process = **ioIterator;

			//Run current I/O burst and increment iterator to run next I/O burst.
			if (process.IsBurstFinished() == false)
			{
				process.DisplayProgress();
				process.Burst(runTime);

				if (process.IsBurstFinished() == true) //Transfer current process to ready queue when its I/O burst is completed.
				{
					process.NextBurst();
					printf("(%s has left I/O Queue!) ", process.GetName());
					//mq.Add(std::move(process), queueNum - 1);
					mq.Add(std::move(process));
					ioIterator = ioList.erase(ioIterator);
				}
				else
				{
					++ioIterator;
				}
			}
		}

		std::cout << "\n\n";

		++runTime;

	} while (mq.IsEmpty() == false || ioList.empty() == false);

	DisplayResults(finishedProcesses, "MLFQ");
}

void DisplayResults(const std::vector<Process*>& finishedProcesses, const char* algName)
{
	printf("%s Final Results:\n", algName);

	for (Process* process : finishedProcesses)
	{
		process->DisplayFinalResults();
	}

	std::cout << "\n";
	std::cout << "[Turnaround Time(Ttr), Waiting Time(Tw), Response Time(Tr)]" << std::endl;
}

int main()
{
	std::vector<int> bursts;
	bursts.reserve(20);

	//README!
	//ONLY ONE ALGORITHM CAN RUN AT A TIME! COMMENT OUT OTHER TWO.
	//COMMENT OUT CODE INSIDE THIS REGION IF YOU WISH TO RUN MLFQ ALGORITHM!
#pragma region FCFS & SJF
	////Creates all processes and inserts them into ready queue
	//ReadyQueue readyQueue;

	//bursts.assign({ 5, 27, 3, 31, 5, 43, 4, 18, 6, 22, 4, 26, 3, 24, 4 }); //225
	//readyQueue.Add(std::move(Process(bursts)));
	//
	//bursts.assign({ 4, 48, 5, 44, 7, 42, 12, 37, 9, 76, 4, 41, 9, 31, 7, 43, 8 }); //427
	//readyQueue.Add(std::move(Process(bursts)));
	//
	//bursts.assign({ 8, 33, 12, 41, 18, 65, 14, 21, 4, 61, 15, 18, 14, 26, 5, 31, 6 }); //392
	//readyQueue.Add(std::move(Process(bursts)));
	//
	//bursts.assign({ 3, 35, 4, 41, 5, 45, 3, 51, 4, 61, 5, 54, 6, 82, 5, 77, 3 }); //484
	//readyQueue.Add(std::move(Process(bursts)));
	//
	//bursts.assign({ 16, 24, 17, 21, 5, 36, 16, 26, 7, 31, 13, 28, 11, 21, 6, 13, 3, 11, 4 }); //309
	//readyQueue.Add(std::move(Process(bursts)));
	//
	//bursts.assign({ 11, 22, 4, 8, 5, 10, 6, 12, 7, 14, 9, 18, 12, 24, 15, 30, 8 }); //215
	//readyQueue.Add(std::move(Process(bursts)));
	//
	//bursts.assign({ 14, 46, 17, 41, 11, 42, 15, 21, 4, 32, 7, 19, 16, 33, 10 }); //328
	//readyQueue.Add(std::move(Process(bursts)));
	//
	//bursts.assign({ 4, 14, 5, 33, 6, 51, 14, 73, 16, 87, 6 }); //309
	//readyQueue.Add(std::move(Process(bursts)));
	//	
	////FOR NOW, RUN ONLY ONE ALGORITHM AT A TIME
	//// 
	////FCFS(readyQueue);
	////SJF(readyQueue);
#pragma endregion

	//COMMENT OUT CODE INSIDE THIS REGION IF YOU WISH TO RUN EITHER FCFS OR SJF ALGORITHMS!
#pragma region MLFQ
	MultiQueue mq;

	bursts.assign({ 5, 27, 3, 31, 5, 43, 4, 18, 6, 22, 4, 26, 3, 24, 4 }); //225
	mq.Add(std::move(Process(bursts)));

	bursts.assign({ 4, 48, 5, 44, 7, 42, 12, 37, 9, 76, 4, 41, 9, 31, 7, 43, 8 }); //427
	mq.Add(std::move(Process(bursts)));

	bursts.assign({ 8, 33, 12, 41, 18, 65, 14, 21, 4, 61, 15, 18, 14, 26, 5, 31, 6 }); //392
	mq.Add(std::move(Process(bursts)));

	bursts.assign({ 3, 35, 4, 41, 5, 45, 3, 51, 4, 61, 5, 54, 6, 82, 5, 77, 3 }); //484
	mq.Add(std::move(Process(bursts)));

	bursts.assign({ 16, 24, 17, 21, 5, 36, 16, 26, 7, 31, 13, 28, 11, 21, 6, 13, 3, 11, 4 }); //309
	mq.Add(std::move(Process(bursts)));

	bursts.assign({ 11, 22, 4, 8, 5, 10, 6, 12, 7, 14, 9, 18, 12, 24, 15, 30, 8 }); //215
	mq.Add(std::move(Process(bursts)));

	bursts.assign({ 14, 46, 17, 41, 11, 42, 15, 21, 4, 32, 7, 19, 16, 33, 10 }); //328
	mq.Add(std::move(Process(bursts)));

	bursts.assign({ 4, 14, 5, 33, 6, 51, 14, 73, 16, 87, 6 }); //309
	mq.Add(std::move(Process(bursts)));

	MLFQ(mq);
#pragma endregion
}