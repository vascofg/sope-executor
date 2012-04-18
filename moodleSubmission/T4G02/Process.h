#ifndef PROCESS_H
#define	PROCESS_H
#include <stdbool.h>
#include <time.h>
#include <sys/resource.h>
#include <stdio.h>
#include <string.h>

/**
 * Process.h - A struct for defining the basic information of a process.
 * Authors: André Freitas and Vasco Gonçalves
 * Emails: p.andrefreitas@gmail.com, vascofg@gmail.com
 */
struct Process {
    char *command;
    int PID;
    bool isRunning;
    struct timeval timeStart;
    struct timeval timeEnd;
    double uTime, sTime;
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

// Process time
void process_beginTime(struct Process *p);
void process_endTime(struct Process *p);
double process_getElapsedTime(struct Process *p);
void process_getUserSystemTimes(struct Process *p);
double process_getUserTime(struct Process *p);
double process_getSysTime(struct Process *p);

// Others methods
void process_print(struct Process *p);
void process_printn(struct Process *p);
void process_terminate(struct Process *p);
bool process_isRunning(struct Process *p);
char * process_toString(struct Process *p);

#endif