#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>

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
void FCFS(Process* processes, int processCount);
void SJF(Process* processes, int processCount);
void SRTF(Process* processes, int processCount);
void P(Process* processes, int processCount);
void RR(Process* processes, int processCount);

// IO subroutines
Process* storeProcesses(int processCount, FILE* inputText); // return an array of all the processes

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

  cout << "There are " << testCases << " test cases all in all in this text file." << endl;

  for (int caseCount = 1; caseCount <= testCases; caseCount++) {
    // the next thing we get from the file is the amount of processes in the test case
    // and the scheduling algorithm to be used
    int processCount;
    char schedulingAlgorithm[10];
    fgets(buffer, sizeof(buffer), inputText);
    sscanf(buffer, "%d %s", &processCount, schedulingAlgorithm);
    memset(buffer, 0, sizeof(buffer));

    // store processes in an array of the process structs
    Process* processQueue = storeProcesses(processCount, inputText);

    // the algo works on that array of process strucuts
    if (strcmp(schedulingAlgorithm, "FCFS") == 0) {
      FCFS(processQueue, processCount);
    } else if (strcmp(schedulingAlgorithm, "SJF") == 0) {
      SJF(processQueue, processCount);
    } else if (strcmp(schedulingAlgorithm, "SRTF") == 0) {
      SRTF(processQueue, processCount);
    } else if (strcmp(schedulingAlgorithm, "P") == 0) {
      P(processQueue, processCount);
    } else {
      RR(processQueue, processCount);
    }

    delete[] processQueue;
  }
  fclose(inputText);
  return 0;
}

// the test cases use one based indexing kekW
// first process index is 1 and the last is X

void FCFS(Process* processes, int processCount) {
  for (int i=0; i < processCount; i++) {
    cout << "Process " << processes[i].processIndex << endl;
    cout << "Arrival Time: " << processes[i].arrivalTime << endl;
    cout << "Burst Time: " << processes[i].burstTime << endl;
    cout << "Priority: " << processes[i].priority << endl;
  }
}

void SJF(Process* processes, int processCount) {
  cout << "hi" << endl;
}

void SRTF(Process* processes, int processCount) {
  cout << "hi" << endl;
}

void P(Process* processes, int processCount) {
  cout << "hi" << endl;
}

void RR(Process* processes, int processCount) {
  cout << "hi" << endl;
}

// this returns an array of Process structs
Process* storeProcesses(int processCount, FILE* inputText) {
  Process* processQueue = new Process[processCount];
  for (int i=0; i < processCount; i++) {
    char buffer[20];
    fgets(buffer, sizeof(buffer), inputText);
    sscanf(buffer, "%d %d %d", &processQueue[i].arrivalTime, &processQueue[i].burstTime, &processQueue[i].priority);
    memset(buffer, 0, sizeof(buffer));
    processQueue[i].processIndex = i+1;
  }
  return processQueue;
}