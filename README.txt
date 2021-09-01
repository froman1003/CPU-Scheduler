This project simulates three CPU scheduling algorithms: First Come First Serve (FCFS), Shortest Job First (SJF), and Multilevel Feedback Queue (MLFQ).
*Currently, this project only has FCFS implemented.

For this project, there are 8 processes, and each process has a number of bursts that need to be executed. The order of the bursts goes like this:
CPU burst, I/O burst (or time), CPU burst, I/O burst,..., CPU burst.

All 8 processes are initially in the ready queue. Only the first process in the ready queue - the process at the front of the queue - will run its CPU burst; the other bursts will wait.
Once the first process has completed its CPU burst, the process will be transferred to the I/O list where its I/O burst will executed; and when that's finished, it will be transferred back to the ready queue.
When one process leaves the ready queue, the next available process is execute.

This whole process will be repeated until every process has completed all of their bursts - CPU and I/O.

FCFS runs the processes in the order they were inserted into the ready queue, nothing special.
