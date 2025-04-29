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

void FCFS(int processCount);
void SJF(int processCount);
void SRTF(int processCount);
void P(int processCount);
void RR(int processCount);

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

    cout << "There are " << processCount << " processes in this test case." << endl;
    cout << "The scheduling algorithm used is: " << string(schedulingAlgorithm) << endl;

    // the now I guess we can call the specific algo?
  }
}
