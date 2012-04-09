#ifndef PROCESS_H
#define	PROCESS_H
#include <stdbool.h>

struct Process {
    bool isRunning;
    char *command;
    int PID;
};

void terminate(struct Process p);

char* getCommand(struct Process p);

void setCommand(struct Process p, char command[]);

bool isRunning(struct Process p);

int getPID(struct Process p);

void setPID(struct Process p, int PID);

#endif