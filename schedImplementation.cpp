#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

// notes from abdiel: think about it on a case-to-case basis, tie-breaker is arrival time
// everytime there's a context-switch/swapping, print to the terminal
// if the swapping occurs because a process has finished, append an X to the output

/*
  --INPUT--
  2
  4 SRTF
  0 50 2
  40 2 3
  20 3 1
  30 55 1
  2 FCFS
  100 10 1
  10 70 1

  --OUTPUT--
  1
  0 1 20
  20 3 3X
  23 1 17
  40 2 2X
  42 1 13X
  55 4 55X
  2
  10 2 70x
  100 1 10X
*/

struct Process {
  int arrivalTime;
  int burstTime;
  int priority;
  int processIndex;
};

// algo logic
void FCFS(vector<Process> &processes, int processCount);
void SJF(vector<Process> &processes, int processCount);
void SRTF(vector<Process> &processes, int processCount);
void P(vector<Process> &processes, int processCount);
void RR(vector<Process> &processes, int processCount);

// subroutines
vector<Process> storeProcesses(int processCount, FILE* inputText); // return a sorted vector of all the processes

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cout << "Usage: " << argv[0] << " input.txt" << endl;
    return -1;
  }

  char *filePath = argv[1];
  FILE *inputText = fopen(filePath, "r");

  // get first line, be able to store the amount of test cases
  char buffer[100];
  fgets(buffer, sizeof(buffer), inputText);
  int testCases = atoi(buffer);
  memset(buffer, 0, sizeof(buffer));

  for (int caseCount = 1; caseCount < testCases+1; caseCount++) {
    // the next thing we get from the file is the amount of processes in the test case
    // and the scheduling algorithm to be used
    cout << caseCount << endl;
    int processCount; // be mindful that this is also the size of processQueue array
    char schedulingAlgorithm[10];
    fgets(buffer, sizeof(buffer), inputText);
    sscanf(buffer, "%d %s", &processCount, schedulingAlgorithm);
    memset(buffer, 0, sizeof(buffer));
    vector<Process> sortedProcesses = storeProcesses(processCount, inputText);

    // the algo works on that array of process strucuts
    if (strcmp(schedulingAlgorithm, "FCFS") == 0) {
      FCFS(sortedProcesses, processCount);
    } else if (strcmp(schedulingAlgorithm, "SJF") == 0) {
      SJF(sortedProcesses, processCount);
    } else if (strcmp(schedulingAlgorithm, "SRTF") == 0) {
      SRTF(sortedProcesses, processCount);
    } else if (strcmp(schedulingAlgorithm, "P") == 0) {
      P(sortedProcesses, processCount);
    } else {
      RR(sortedProcesses, processCount);
    }
  }
  fclose(inputText);
  return 0;
}

// the test cases use one based indexing kekW
// first process index is 1 and the last is X

// output format
// test case number
// running process 1
// running process 2
// ...
// end process X
// test case number + 1

// sample for FCFS (without the case number)
// INPUT
// 2 FCFS
// 100 10 1 (arrival time, burst, priority)
// 10 70 1
// OUTPUT
// 10 2 70X (time elapsed so far, process index, CPU time used/burst time)
// 100 1 10X

// these functions are passed in SORTED VECTORS of process structs
// maybe we can do a rotating queue, process that finish are popped
// our execution condition is while the queue still has shit inside

void FCFS(vector<Process> &processes, int processCount) {
  for (int i = 0; i < processCount; i++) {
    cout << "Process " << processes[i].processIndex << " || ";
    cout << "Arrival Time: " << processes[i].arrivalTime << " ";
    cout << "Burst Time: " << processes[i].burstTime << " ";
    cout << "Priority: " << processes[i].priority << endl;
  }
}

void SJF(vector<Process> &processes, int processCount) {
  cout << "hi" << endl;
}

void SRTF(vector<Process> &processes, int processCount) {
  cout << "hi" << endl;
}

void P(vector<Process> &processes, int processCount) {
  cout << "hi" << endl;
}

void RR(vector<Process> &processes, int processCount) {
  cout << "hi" << endl;
}

// returns a sorted vector of the processes in block
vector<Process> storeProcesses(int processCount, FILE* inputText) {
  vector<Process> processes;
  for (int i=0; i < processCount; i++) {
    char buffer[20];
    Process temp;
    fgets(buffer, sizeof(buffer), inputText);
    sscanf(buffer, "%d %d %d", &temp.arrivalTime, &temp.burstTime, &temp.priority);
    memset(buffer, 0, sizeof(buffer));
    temp.processIndex = i+1;
    processes.push_back(temp);
  }
  sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
    return a.arrivalTime < b.arrivalTime;
  });
  return processes;
}