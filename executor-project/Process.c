#include "Process.h"
#include <stdbool.h>

void terminate(struct Process p) {
    p.isRunning = false;
}

char* getCommand(struct Process p) {
    return p.command;
}

void setCommand(struct Process p, char command[]) {
    p.command = command;
}

bool isRunning(struct Process p) {
    return p.isRunning;
}

int getPID(struct Process p) {
    return p.PID;
}

void setPID(struct Process p, int PID) {
    p.PID = PID;
}