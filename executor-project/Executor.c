#include "Executor.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_LINE 100
#define MAX_PROCESSES 20
#define MAX_ACTIVITIES 50
void executor_create(struct Executor *e,char* logFileName, char* errorFileName){
    e->logFileName=logFileName;
    e->errorFileName=errorFileName;
    
    e->runningProcesses=0;
    e->terminatedProcesses=0;
    
    e->processLastIndex=-1;
    e->activityLastIndex=-1;
    e->processes=malloc(sizeof(struct Process *) * MAX_PROCESSES);
    e->activities=malloc(sizeof(struct Activity *) * MAX_ACTIVITIES);
}

void executor_run(struct Executor *e){
    char option;
    do{
        executor_printHeader(e);
        option=executor_receiveOrder(e);
        switch(option){
            case 'A': executor_launch(e); break;
            case 'I': executor_inform(e); break;
            case 'T': executor_terminate(e); break;
            case 'S': executor_exit(e); break;
            default: printf("Opção inválida!\n");
        }
        
    }while(option!='S');
}

void executor_printHeader(struct Executor *e){
    printf("*** Executor de comandos ***\n");
    printf("Registo: %d comandos a decorrer; %d terminados\n", e->runningProcesses,e->terminatedProcesses);
    printf("Controlos: (A)rrancar – (I)nformar – (T)erminar - (S)air\n");
}

char executor_receiveOrder(struct Executor *e){
    char input[MAX_LINE];
    do{
        printf("Ordem: ");
        gets(input);
    } while(strlen(input)!=1);
    input[0]=toupper(input[0]);
    return input[0];
}

void executor_launch(struct Executor *e){
    printf("Launch\n");
}

void executor_inform(struct Executor *e){
    printf("Inform\n");
}
void executor_terminate(struct Executor *e){
    printf("Terminate\n");
}

void executor_exit(struct Executor *e){
    printf("Exit\n");
}

void executor_addProcess(struct Executor *e,struct Process *p){
    e->processLastIndex++;
    e->processes[e->processLastIndex]=p;
}

void executor_addActivity(struct Executor *e,struct Activity *a){
    e->activityLastIndex++;
    e->activities[e->activityLastIndex]=a;
}