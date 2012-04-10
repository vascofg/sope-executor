#ifndef PROCESS_H
#define	PROCESS_H
#include <stdbool.h>
/**
 * Process.h - A struct for defining the basic information of a process.
 */ 
struct Process {
    char *command;
    int PID;
    bool isRunning;
};
/**
 * Creates a new process with the necessary information
 * @param p the process pointer
 * @param isRunning the running boolean value
 * @param command the command given
 * @param PID the Process identificador in Linux
 */
void process_create(struct Process *p, char *command, int PID);

// Set methods
void process_setCommand(struct Process *p, char *command);
void process_setRunning(struct Process *p, bool process_isRunning);
void process_setPID(struct Process *p, int PID);

// Get Methods
char * process_getCommand(struct Process *p);
int process_getPID(struct Process *p);
bool process_getRunning(struct Process *p);

// Others methods
void process_print(struct Process *p);
void process_printn(struct Process *p);
void process_terminate(struct Process *p);
bool process_isRunning(struct Process *p);

#endif