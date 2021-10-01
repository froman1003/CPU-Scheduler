WHAT IS THIS PROJECT
-------------------------------------------------------------------------------------------------------------------
This project simulates three CPU scheduling algorithms: First Come First Serve (FCFS), Shortest Job First (SJF), and Multilevel Feedback Queue (MLFQ).

For this project, there are 8 processes, and each process has a number of bursts that need to be executed. The order of the bursts goes like this:
CPU burst, I/O burst (or time), CPU burst, I/O burst,..., CPU burst.

HOW IT WORKS
-------------------------------------------------------------------------------------------------------------------
All 8 processes are initially in the ready queue. Only the first process in the ready queue - the process at the front of the queue - will run its CPU burst; the other bursts will wait.
Once the first process has completed its CPU burst, the process will be transferred to the I/O list where its I/O burst will execute; and when that's finished, it will be transferred back to the ready queue.
When one process leaves the ready queue, the next available process is executed.

This whole process will be repeated until every process has completed all of their bursts - CPU and I/O.

HOW EACH ALGORITHM WORKS
-------------------------------------------------------------------------------------------------------------------
FCFS runs the processes in the order they were inserted into the ready queue, nothing special.

For SJF, initially, all processes in the ready queue are sorted from shortest burst to longest burst. Ready queue is sorted whenever a process is inserted back in from the I/O list.

In a MLFQ, there are a number of queues - in this case there are three - from highest priority to lowest. If a process does not finish its burst before the time allotted by the queue (only the first two
queues do this), then the process is transfered to the next lowest queue. A low priority queue is only executed when the higher priority queue(s) are finished/empty. A process in a lower priority queue,
which is running, can be preempted by a process that becomes available in a higher priortiy queue. Processes in this simulation can only be downgraded to lower priority queues, not upgraded to higher ones.
