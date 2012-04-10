#include "Executor.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#define MAX_LINE 100
#define MAX_PROCESSES 20
#define MAX_ACTIVITIES 50

void executor_create(struct Executor *e, char* logFileName, char* errorFileName) {
    e->logFileName = logFileName;
    e->errorFileName = errorFileName;

    e->runningProcesses = 0;
    e->terminatedProcesses = 0;

    e->processLastIndex = -1;
    e->processes = malloc(sizeof (struct Process *) * MAX_PROCESSES);
}

void handler(int sig) {
    pid_t pid;

    pid = wait(NULL);

    printf("Pid %d exited.\n", pid);
}

void executor_run(struct Executor *e) {
    char option;
    signal(SIGCHLD, handler); 
    do {
        executor_printHeader(e);
        option = executor_receiveOrder(e);
        switch (option) {
            case 'A': executor_launch(e);
                break;
            case 'I': executor_inform(e);
                break;
            case 'T': executor_terminate(e);
                break;
            case 'S': executor_exit(e);
                break;
            default: printf("Opção inválida!\n");
        }

    } while (option != 'S');
}

void executor_printHeader(struct Executor *e) {
    printf("*** Executor de comandos ***\n");
    printf("Registo: %d comandos a decorrer; %d terminados\n", e->runningProcesses, e->terminatedProcesses);
    printf("Controlos: (A)rrancar – (I)nformar – (T)erminar - (S)air\n");
}

char executor_receiveOrder(struct Executor *e) {
    char input[MAX_LINE];
    do {
        printf("Ordem: ");
        gets(input);
    } while (strlen(input) != 1);
    input[0] = toupper(input[0]);
    system("clear");
    return input[0];
}

void executor_launch(struct Executor *e) {
    char cmd[100];
    pid_t pid;

    // (1) get the command
    printf("> ");
    gets(cmd);
    //(2) fork
    // child
    if ((pid = fork()) == 0) {
        execlp("xterm", "xterm", "-hold", "-e", cmd, NULL);
    }        // failed
    else if (pid < 0) {
        perror("fork(): ");
    }
        // parent
    else {
        // Create the information of the process
        struct Process *p = malloc(sizeof (struct Process));
        process_create(p, cmd, pid);
        e->runningProcesses++;
        executor_addProcess(e, p);
        process_printn(p);

    }

}

void executor_inform(struct Executor *e) {
    printf("Inform\n");
}

void executor_printActiveProcesses(struct Executor *e) {
    int totalProcesses = e->processLastIndex + 1;
    int i;
    for (i = 0; i < totalProcesses; i++) {
        if (process_isRunning(e->processes[i])) {
            process_printn(e->processes[i]);
        }
    }
}

void executor_terminate(struct Executor *e) {
    printf("Choose a PID to terminate:\n");
    executor_printActiveProcesses(e);
    printf("PID> ");
    int pid;
    scanf("%d", &pid);
    if (kill(pid, SIGKILL) < 0) {
        perror("Kill()");
    };
    process_terminate(executor_getProcessbyPID(e, pid));
    e->terminatedProcesses++;
    e->runningProcesses--;

}

void executor_exit(struct Executor *e) {
    printf("Exit\n");
}

void executor_addProcess(struct Executor *e, struct Process *p) {
    e->processLastIndex++;
    e->processes[e->processLastIndex] = p;
}

struct Process * executor_getProcessbyPID(struct Executor *e, int pid) {
    int totalProcesses = e->processLastIndex + 1;
    int i;
    for (i = 0; i < totalProcesses; i++)
        if (process_getPID(e->processes[i]) == pid)
            return e->processes[i];

    return NULL;
}