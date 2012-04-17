#include "Process.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h> 
#include <unistd.h> 

#define MAX_STRING_LENGTH 100

void process_create(struct Process *p, char *command, int PID) {
    p->PID = PID;
    p->isRunning = 1;
    p->command = malloc(sizeof (char) *MAX_STRING_LENGTH);
    strcpy(p->command, command);
    process_beginTime(p);
}

void process_setCommand(struct Process *p, char *command) {
    p->command = command;
}

void process_setRunning(struct Process *p, bool process_isRunning) {
    p->isRunning = process_isRunning;
}

void process_setPID(struct Process *p, int PID) {
    p->PID = PID;
}

char * process_getCommand(struct Process *p) {
    return p->command;
}

int process_getPID(struct Process *p) {
    return p->PID;
}

bool process_getRunning(struct Process *p) {
    return p->isRunning;
}

void process_terminate(struct Process *p) {
    p->isRunning = false;
    process_endTime(p);
}

bool process_isRunning(struct Process *p) {
    return p->isRunning;
}

void process_print(struct Process *p) {
    printf("Process \"%s\" with PID %d ", p->command, p->PID);
}

void process_printn(struct Process *p) {
    process_print(p);
    printf("\n");
}

char * process_toString(struct Process *p) {
    char * buffer = malloc(sizeof (char) *MAX_STRING_LENGTH);
    sprintf(buffer, "Process \"%s\" with PID %d", p->command, p->PID);
    return buffer;
}

void process_beginTime(struct Process *p) {
    gettimeofday(&p->timeStart, NULL);
}

void process_endTime(struct Process *p) {
    gettimeofday(&p->timeEnd, NULL);
}

//FUNCTION BASED ON THE ONE FOUND HERE: https://github.com/fho/code_snippets/blob/master/c/getusage.c

void process_getUserSystemTimes(struct Process *p) {
    long unsigned int *utime_ticks = malloc(sizeof (unsigned int));
    long unsigned int *stime_ticks = malloc(sizeof (unsigned int));
    //convert pid to string
    char pid_s[20];
    snprintf(pid_s, sizeof (pid_s), "%d", p->PID);
    char stat_filepath[30] = "/proc/";
    strncat(stat_filepath, pid_s,
            sizeof (stat_filepath) - strlen(stat_filepath) - 1);
    strncat(stat_filepath, "/stat", sizeof (stat_filepath) -
            strlen(stat_filepath) - 1);

    //Open /proc/stat and /proc/$pid/stat fds successive(dont want that cpu
    //ticks increases too much during measurements)
    FILE *fpstat = fopen(stat_filepath, "r");
    if (fpstat == NULL) {
        perror("FOPEN ERROR ");
    }

    //read values from /proc/pid/stat
    if (fscanf(fpstat, "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu"
            "%lu %*d %*d %*d %*d %*d %*d %*u %*u %*d",
            utime_ticks, stime_ticks) == EOF) {
        fclose(fpstat);
        perror("FILE READ ERROR ");
    }
    fclose(fpstat);
    //DIVIDE TICKS BY NUM TICKS PER SEC
    p->uTime = *utime_ticks / (double) sysconf(_SC_CLK_TCK);
    p->sTime = *stime_ticks / (double) sysconf(_SC_CLK_TCK);
}

double process_getUserTime(struct Process *p) {
    return p->uTime;
}

double process_getSysTime(struct Process *p) {
    return p->sTime;
}

double process_getElapsedTime(struct Process *p) {
    return ((p->timeEnd.tv_sec + (p->timeEnd.tv_usec / 1000000.0))-(p->timeStart.tv_sec + (p->timeStart.tv_usec / 1000000.0)));
}