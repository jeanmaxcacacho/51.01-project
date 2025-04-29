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

int main(void) {
    char buffer[15];
    string output;
    FILE* input = fopen("test.txt", "r");

    while (fgets(buffer, sizeof(buffer), input)) {
        output = string(buffer);
        memset(buffer, 0, sizeof(buffer));
        cout << output;
    }
    buffer[0] = '\n';
    output = string(buffer);
    cout << output;
    memset(buffer, 0, sizeof(buffer));
    return 0;
}
