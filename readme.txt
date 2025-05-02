The submitted program is a *.cpp source file that can be compiled to
accept a text input of process test cases and their associated algori-
thms. In this file we implement the following scheduling algorithms:

1. first come first serve
2. shortest job first
3. shortest remaining time first
4. preemptive priority
5. arrival-first round robin

The program has been segmented into subroutines with wherever logical
and possible. "I/O" subroutines for transferring data between functions
were written: storeProcesses, setBurstsLeft etc. Processes exist as their
own separate struct with the following member attributes:

-- base metrics needed
1. arrivalTime
2. burstTime
3. priority
4. processIndex

-- performance reporting metrics
5. firstResponse
6. terminationTime

-- for preemptive algo
7. burstsLeft

The algorithms work by working on vectors/queues of these structs, arranging
the front properly. Or at least all of those algorithms except FCFS, processes
are already sorted by arrival time when they are passed into the algorithm.

For all algorithms except FCFS, an intermediate "readyQueue" vector is instantiated
to handle process dispatching. This vector grows in importance with the preemptive
algorithms. At the end of each algorithm function we call the reportPerformance subroutine
to display all the desired metrics.