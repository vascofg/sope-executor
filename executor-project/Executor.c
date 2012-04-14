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
void cls(void) {
  printf("%c[2J",27);
}

void executor_create(struct Executor *e, char* logFileName, char* errorFileName) {
    // (1) Set the log and error files name
    e->logFileName = logFileName;
    e->errorFileName = errorFileName;
    
    // (2) Counters start at 0
    e->runningProcesses = 0;
    e->terminatedProcesses = 0;
    
    // (3) Since we don't have any processes running at creation, the last index is -1
    e->processLastIndex = -1;
    e->processes = malloc(sizeof (struct Process *) * MAX_PROCESSES);
    
    // (4) Set the global variable of the current executor to this executor
    currentExecutor = e;
}

void executor_sigchildHandler(int sig) {
    // (1) get the pid
    pid_t pid;
    pid = wait(NULL);
    
    // (2) Terminate the process in the executor
    process_terminate(executor_getProcessbyPID(currentExecutor, pid));
    
    // (3) Update counters
    currentExecutor->terminatedProcesses++;
    currentExecutor->runningProcesses--;
    
    // (4) Alert user
    printf("\nProcess with %d terminated.\n", pid);
    
    // (5) Add log
    char * buffer=malloc(sizeof(char *)*MAX_LINE);
    sprintf(buffer, "%s terminated",process_toString(executor_getProcessbyPID(currentExecutor, pid)));
    executor_addLog(currentExecutor,buffer);
    
    sleep(2);
    
}

void executor_run(struct Executor *e) {
    // (1) Setup the SIGCHILD handler
    signal(SIGCHLD, executor_sigchildHandler);
    
    // (2) Executor loop
    char option;
    do {
        cls();
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
    cls();
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
    }// failed
    else if (pid < 0) {
        perror("fork(): ");
    }// parent
    else {
        // Create the information of the process
        struct Process *p = malloc(sizeof (struct Process));
        process_create(p, cmd, pid);
        e->runningProcesses++;
        executor_addProcess(e, p);
        process_printn(p);

        //  Add log
        char * buffer = malloc(sizeof (char *) *MAX_LINE);
        sprintf(buffer, "%s created", process_toString(p));
        executor_addLog(e, buffer);


        sleep(2);

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
    if (e->runningProcesses > 0) {
        printf("Choose a PID to terminate:\n");
        executor_printActiveProcesses(e);
        int pid;
        char input[100];
        do {
            printf("PID> ");
            gets(input);
            pid = atoi(input);
        } while (executor_getProcessbyPID(e, pid) == NULL);

        if (kill(pid, SIGKILL) < 0) {
            perror("Kill()");
        };
        sleep(1); // wait feedback
    } else {
        printf("There are no processes running!\n");
    }


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

void executor_addLog(struct Executor *e, char *log) {

    // (1) Open log file in append mode
    int logFileDes;
    logFileDes = open(e->logFileName, O_RDWR | O_CREAT | O_APPEND, 0777);

    // Error Handling
    if (logFileDes < 0) {
        perror("logFileName: ");
        exit(1);
    }
    strcat(log, "\n");
    // (2) Append data
    write(logFileDes, log, strlen(log));

}