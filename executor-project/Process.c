#include "Process.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STRING_LENGTH 100
void process_create(struct Process *p,char *command, int PID){
    p->PID=PID;
    p->isRunning=1;
    p->command=malloc(sizeof(char)*MAX_STRING_LENGTH);
    strcpy(p->command,command);
}

void process_setCommand(struct Process *p, char *command){
    p->command=command;
}

void process_setRunning(struct Process *p, bool process_isRunning){
    p->isRunning=process_isRunning;
}

void process_setPID(struct Process *p, int PID){
    p->PID=PID;
}

char * process_getCommand(struct Process *p){
    return p->command;
}

int process_getPID(struct Process *p){
    return p->PID;
}

bool process_getRunning(struct Process *p){
    return p->isRunning;
}

void process_terminate(struct Process *p){
    p->isRunning=false;
}

bool process_isRunning(struct Process *p){
    return p->isRunning;
}

void process_print(struct Process *p){
    printf("Process \"%s\" with PID %d ",p->command, p->PID);
}

void process_printn(struct Process *p){
    process_print(p);
    printf("\n");
}

char * process_toString(struct Process *p){
    char * buffer=malloc(sizeof(char)*MAX_STRING_LENGTH);
    sprintf (buffer,"Process \"%s\" with PID %d",p->command, p->PID);
    return buffer;
}