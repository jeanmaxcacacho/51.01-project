#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <algorithm>
#include <vector>
#include <deque>
#include <iomanip>

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

// TODO: added struct member attribute for block report (e.g. first response, termination)
struct Process {
  int arrivalTime;
  int burstTime;
  int priority;
  int processIndex;

  // for reporting
  int firstResponse;
  int terminationTime;

  // for the preemptive scheduling algos
  int burstsLeft;
};

// algo logic
void FCFS(vector<Process> &processes, int processCount);
void SJF(vector<Process> &processes, int processCount);
void SRTF(vector<Process> &processes, int processCount);
void P(vector<Process> &processes, int processCount);
void RR(vector<Process> &processes, int processCount, int rrTimeSlice);

// subroutines
vector<Process> storeProcesses(int processCount, FILE* inputText); // return a sorted vector of all the processes
void setBurstsLeft(vector<Process> &processVector); // lmfao kek
void reportPerformance(vector<Process> &processVector, int processCount, int cpuActiveTime, int totalExecTime);

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
    int processCount; // be mindful that this is also the size of the processes vector
    int rrTimeSlice;
    char schedulingAlgorithm[10];
    fgets(buffer, sizeof(buffer), inputText);
    sscanf(buffer, "%d %s %d", &processCount, schedulingAlgorithm, &rrTimeSlice); // so apparently this works?
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
      RR(sortedProcesses, processCount, rrTimeSlice);
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

// these functions are passed in SORTED VECTORS of process structs
// maybe we can do a rotating queue, process that finish are popped
// our execution condition is while the queue still has shit inside

// sample for FCFS (without the case number)
// INPUT
// 2 FCFS
// 100 10 1 (arrival time, burst, priority)
// 10 70 1
// OUTPUT
// 10 2 70X (time elapsed so far, process index, CPU time used/burst time)
// 100 1 10X
void FCFS(vector<Process> &processes, int processCount) {
  Process currentProcess = processes.front();
  int burstsLeft = currentProcess.burstTime;
  int currentTime = 0;

  while (!processes.empty()) {
    // only write to terminal when there is a context switch
    // in the case of FCFS context switches only happen when
    // the current process finishes execution
    if (currentTime < currentProcess.arrivalTime) {
      currentTime = currentProcess.arrivalTime;
      cout << currentProcess.arrivalTime;
    }
    if (burstsLeft == 0) {
      cout << " " << currentProcess.processIndex << " " << currentProcess.burstTime << "X\n";
      processes.erase(processes.begin());
      currentProcess = processes.front();
      burstsLeft = currentProcess.burstTime;
    }
    currentTime++;
    burstsLeft--;
  }
}

void SJF(vector<Process> &processes, int processCount) {
  vector<Process> readyQueue;
  vector<Process> completeProcesses;
  int currentTime = 0;
  bool cpuIdle = true;
  Process currentProcess;
  int burstsLeft = 0;
  int cpuActiveTime = 0;

  for (auto &p : processes) {
    p.firstResponse = -1;
  }

  while (!processes.empty() || !readyQueue.empty() || !cpuIdle) {
    // check for new arrivals
    int i = 0;
    while (i < processes.size()) {
      if (processes[i].arrivalTime <= currentTime) {
        readyQueue.push_back(processes[i]);
        processes.erase(processes.begin() + i);
      } else {
        ++i;
      }
    }

    // cpu is idle and there are ready processes, schedule one
    if (cpuIdle && !readyQueue.empty()) {
      sort(readyQueue.begin(), readyQueue.end(), [](const Process &a, const Process &b) {
        if (a.burstTime == b.burstTime)
          return a.arrivalTime < b.arrivalTime;
        return a.burstTime < b.burstTime;
      });

      currentProcess = readyQueue.front();
      readyQueue.erase(readyQueue.begin());

      if (currentProcess.firstResponse == -1) {
        currentProcess.firstResponse = currentTime;
      }

      cpuIdle = false;
      burstsLeft = currentProcess.burstTime;

      cout << currentTime << " " << currentProcess.processIndex << " " << currentProcess.burstTime << "X\n";

      currentTime += burstsLeft;
      cpuActiveTime += burstsLeft;

      currentProcess.terminationTime = currentTime;
      completeProcesses.push_back(currentProcess);
      burstsLeft = 0;
      cpuIdle = true;
    } else if (cpuIdle && readyQueue.empty() && !processes.empty()) {
      currentTime = processes.front().arrivalTime;
    }
  }
  reportPerformance(completeProcesses, processCount, cpuActiveTime, currentTime);
}

void SRTF(vector<Process> &processes, int processCount) {
  setBurstsLeft(processes);

  vector<Process> readyQueue;
  int currentTime = 0;
  int burstStartTime = 0;
  Process currentProcess;
  bool isRunning = false;
  vector<Process> completeProcesses;
  int cpuActiveTime = 0;

  for (auto &p : processes) {
    p.firstResponse = -1; // fixed
  }

  while (!readyQueue.empty() || !processes.empty() || isRunning) {
    // Add any arriving processes to the ready queue
    int i = 0;
    while (i < processes.size()) {
      if (processes[i].arrivalTime <= currentTime) {
        readyQueue.push_back(processes[i]);
        processes.erase(processes.begin() + i);
      } else {
        ++i;
      }
    }

    if (!readyQueue.empty()) {
      // Sort by shortest remaining time
      sort(readyQueue.begin(), readyQueue.end(), [](const Process &a, const Process &b) {
        if (a.burstsLeft == b.burstsLeft)
          return a.arrivalTime < b.arrivalTime;
        return a.burstsLeft < b.burstsLeft;
      });

      Process nextProcess = readyQueue.front();

      // Detect context switch (including first run)
      if (!isRunning || currentProcess.processIndex != nextProcess.processIndex) {
        if (isRunning) {
          int timeUsed = currentTime - burstStartTime;
          cout << burstStartTime << " " << currentProcess.processIndex << " " << timeUsed;
          if (currentProcess.burstsLeft == 0)
            cout << "X";
          cout << "\n";
        }
        burstStartTime = currentTime;
        currentProcess = nextProcess;
        isRunning = true;

        for (auto &p : readyQueue) {
          if (p.processIndex == currentProcess.processIndex && p.firstResponse == -1) {
            p.firstResponse = currentTime;
            break;
          }
        }
      }

      // Execute one unit of time
      currentProcess.burstsLeft--;
      currentTime++;
      cpuActiveTime++;

      // Sync currentProcess.burstsLeft to readyQueue
      for (auto &p : readyQueue) {
        if (p.processIndex == currentProcess.processIndex) {
          p.burstsLeft = currentProcess.burstsLeft;
          break;
        }
      }

      // If process finished this time unit, print and remove
      if (currentProcess.burstsLeft == 0) {
        int timeUsed = currentTime - burstStartTime;
        cout << burstStartTime << " " << currentProcess.processIndex << " " << timeUsed << "X\n";

        for (auto it = readyQueue.begin(); it != readyQueue.end(); ++it) {
          if (it->processIndex == currentProcess.processIndex) {
            it->terminationTime = currentTime;
            completeProcesses.push_back(*it);
            readyQueue.erase(it);
            break;
          }
        }

        isRunning = false;
      }

    } else {
      // CPU is idle
      currentTime++;
    }
  }

  reportPerformance(completeProcesses, processCount, cpuActiveTime, currentTime);
}

void P(vector<Process> &processes, int processCount) {
  setBurstsLeft(processes);  // Initialize burstsLeft
  vector<Process> readyQueue;
  int currentTime = 0;
  Process currentProcess;
  bool isRunning = false;
  int burstStartTime = 0;
  int cpuActiveTime = 0;  // Track total CPU active time
  vector<Process> completeProcesses;

  // Initialize firstResponse for all processes
  for (auto &p : processes) {
    p.firstResponse = -1;  // No response yet
  }

  while (!readyQueue.empty() || !processes.empty() || isRunning) {
    // Add new arrivals to the ready queue
    int i = 0;
    while (i < processes.size()) {
      if (processes[i].arrivalTime <= currentTime) {
        readyQueue.push_back(processes[i]);
        processes.erase(processes.begin() + i);
      } else {
        ++i;
      }
    }

    if (!readyQueue.empty()) {
      // Sort by descending priority, arrival time, then process index
      sort(readyQueue.begin(), readyQueue.end(), [](const Process &a, const Process &b) {
        if (a.priority == b.priority) {
          if (a.arrivalTime == b.arrivalTime)
            return a.processIndex < b.processIndex;
          return a.arrivalTime < b.arrivalTime;
        }
        return a.priority > b.priority;  // higher value = higher priority
      });

      Process nextProcess = readyQueue.front();

      // Context switch if new process is different
      if (!isRunning || nextProcess.processIndex != currentProcess.processIndex) {
        // Only print switch-out if process wasn't just finished
        if (isRunning && currentProcess.burstsLeft > 0) {
          int timeUsed = currentTime - burstStartTime;
          cout << burstStartTime << " " << currentProcess.processIndex << " " << timeUsed << "\n";
        }

        burstStartTime = currentTime;
        currentProcess = nextProcess;

        // Set first response time if not already set
        if (currentProcess.firstResponse == -1) {
          currentProcess.firstResponse = currentTime;
        }

        isRunning = true;
      }

      // Run current process for 1 time unit
      currentProcess.burstsLeft--;
      currentTime++;
      cpuActiveTime++;  // Increment the CPU active time

      // Update current process in ready queue
      for (auto &p : readyQueue) {
        if (p.processIndex == currentProcess.processIndex) {
          p.burstsLeft = currentProcess.burstsLeft;
          break;
        }
      }

      // If process finishes now, print and remove it
      if (currentProcess.burstsLeft == 0) {
        int timeUsed = currentTime - burstStartTime;
        cout << burstStartTime << " " << currentProcess.processIndex << " " << timeUsed << "X\n";

        // Set termination time when the process finishes
        currentProcess.terminationTime = currentTime;
        completeProcesses.push_back(currentProcess);  // Store completed process

        // Remove from readyQueue
        readyQueue.erase(remove_if(readyQueue.begin(), readyQueue.end(),
                                   [&](const Process &p) { return p.processIndex == currentProcess.processIndex; }),
                         readyQueue.end());
        isRunning = false;
      }

    } else {
      // CPU is idle
      currentTime++;
    }
  }

  // Sort completed processes by process index
  sort(completeProcesses.begin(), completeProcesses.end(),
       [](const Process &a, const Process &b) {
           return a.processIndex < b.processIndex;
       });

  // Pass the total CPU active time and current time (as total execution time) to reportPerformance
  reportPerformance(completeProcesses, processCount, cpuActiveTime, currentTime);
}

void RR(vector<Process> &processes, int processCount, int rrTimeSlice) {
  setBurstsLeft(processes);
  deque<Process> readyQueue;
  vector<Process> completeProcesses;
  int currentTime = 0;
  int burstStartTime = 0;
  int cpuActiveTime = 0;  // Track total active time of the CPU

  // Initialize firstResponse for all processes
  for (auto &p : processes) {
    p.firstResponse = -1;  // No response yet
  }

  while (!readyQueue.empty() || !processes.empty()) {
    // Add all newly arrived processes to the queue
    int i = 0;
    while (i < processes.size()) {
      if (processes[i].arrivalTime <= currentTime) {
        readyQueue.push_back(processes[i]);
        processes.erase(processes.begin() + i);
      } else {
        ++i;
      }
    }

    if (!readyQueue.empty()) {
      Process currentProcess = readyQueue.front();
      readyQueue.pop_front();

      // Set first response time if not already set
      if (currentProcess.firstResponse == -1) {
        currentProcess.firstResponse = currentTime;
      }

      burstStartTime = currentTime;
      int timeToRun = min(rrTimeSlice, currentProcess.burstsLeft);
      currentTime += timeToRun;
      currentProcess.burstsLeft -= timeToRun;

      cpuActiveTime += timeToRun;  // Increment the CPU active time

      cout << burstStartTime << " " << currentProcess.processIndex << " " << timeToRun;
      if (currentProcess.burstsLeft == 0) {
        cout << "X";  // Process finished
        // Set termination time when the process finishes
        currentProcess.terminationTime = currentTime;
        completeProcesses.push_back(currentProcess);  // Store completed process
      }
      cout << "\n";

      // Newly arrived processes that came in during this time slice
      i = 0;
      while (i < processes.size()) {
        if (processes[i].arrivalTime <= currentTime) {
          readyQueue.push_front(processes[i]);
          processes.erase(processes.begin() + i);
        } else {
          ++i;
        }
      }

      // If process still has remaining time, requeue it
      if (currentProcess.burstsLeft > 0) {
        readyQueue.push_back(currentProcess);
      }

    } else {
      currentTime++;  // Nothing is in the queue but processes haven't arrived yet, advance time
    }
  }

  // Sort completed processes by process index
  sort(completeProcesses.begin(), completeProcesses.end(),
       [](const Process &a, const Process &b) {
           return a.processIndex < b.processIndex;
       });

  // You can now pass cpuActiveTime and currentTime (as totalExecTime) to reportPerformance
  reportPerformance(completeProcesses, processCount, cpuActiveTime, currentTime);
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

void setBurstsLeft(vector<Process> &processVector) {
  for (int i=0; i < processVector.size(); i++) {
    processVector[i].burstsLeft = processVector[i].burstTime;
  }
}

void reportPerformance(vector<Process> &processVector, int processCount, int cpuActiveTime, int totalExecTime) {
  cout << fixed << setprecision(2);

  float cpuUtil = (static_cast<float>(cpuActiveTime) / totalExecTime) * 100;
  float throughput = static_cast<float>(processCount) / cpuActiveTime;

  vector<int> processTurnaroundTimes;
  int totalTurnaroundTime = 0;
  for (int i = 0; i < processVector.size(); i++) {
    int tat = processVector[i].terminationTime - processVector[i].arrivalTime;
    processTurnaroundTimes.push_back(tat);
    totalTurnaroundTime += tat;
  }
  float avgTurnaroundTime = static_cast<float>(totalTurnaroundTime) / processCount;

  vector<int> processWaitingTimes;
  int totalWaitingTime = 0;
  for (int i = 0; i < processTurnaroundTimes.size(); i++) {
    int wt = processTurnaroundTimes[i] - processVector[i].burstTime;
    processWaitingTimes.push_back(wt);
    totalWaitingTime += wt;
  }
  float avgProcessWaitingTime = static_cast<float>(totalWaitingTime) / processCount;

  vector<int> processResponseTimes;
  int totalResponseTime = 0;
  for (int i = 0; i < processVector.size(); i++) {
    int rt = processVector[i].firstResponse - processVector[i].arrivalTime;
    processResponseTimes.push_back(rt);
    totalResponseTime += rt;
  }
  float avgResponseTime = static_cast<float>(totalResponseTime) / processCount;

  cout << "CPU Utilization: " << cpuUtil << "%" << endl;
  cout << "Throughput: " << throughput << " processes/ns" << endl;

  cout << "Average waiting time: " << avgProcessWaitingTime << "ns\n";
  for (int i = 0; i < processVector.size(); i++) {
    cout << "    Process " << i + 1 << " waiting time: " << processWaitingTimes[i] << "ns\n";
  }

  cout << "Average turnaround time: " << avgTurnaroundTime << "ns\n";
  for (int i = 0; i < processVector.size(); i++) {
    cout << "    Process " << i + 1 << " turnaround time: " << processTurnaroundTimes[i] << "ns\n";
  }

  cout << "Average response time: " << avgResponseTime << "ns\n";
  for (int i = 0; i < processVector.size(); i++) {
    cout << "    Process " << i + 1 << " response time: " << processResponseTimes[i] << "ns\n";
  }

}