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
#include <unistd.h>
#include <sys/times.h>

#define MAX_LINE 100
#define MAX_PROCESSES 20
void cls(void) {
    // Credit goes to http://snipplr.com/view/15319/hacky-screen-clearing-through-printf/
   printf("\033[2J\033[0;0f");
   printf("\033[%d;%df", 0, 0);
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
    
    // (5) Create the log file
     if(open(e->logFileName, O_CREAT | O_TRUNC, 0777)<0)
        perror("logFile");
    
    // (6) Create the error file and redirects standard error
    if((e->errorDescriptor=open(e->errorFileName, O_CREAT | O_TRUNC, 0777))<0)
        perror("errorFile");
}

void executor_sigchildHandler(int sig) {
    // (1) get the pid and the pointer to the struct of the process
    pid_t pid;
    pid = wait(NULL);
    struct Process *thisProcess=executor_getProcessbyPID(currentExecutor, pid);
    
    // (2) Terminate the process in the executor and register the endtime
    process_terminate(thisProcess);

    
    // (3) Update counters
    currentExecutor->terminatedProcesses++;
    currentExecutor->runningProcesses--;
    
    // (4) Alert user
    cls();
    printf("\nProcess with PID %d terminated.\n", pid);
    
    // (5) Time measure
    double realTime;
    realTime=process_getElapsedTime(thisProcess);
    
    // (5) Add log
    char * buffer=malloc(sizeof(char *)*MAX_LINE);
  
    sprintf(buffer, "%s terminated\nRan for: %fs",process_toString(thisProcess),realTime);
    executor_addLog(currentExecutor,buffer);
     
    // (6) Refresh the executor
    fflush(stdin);
    sleep(3);
    cls();
    executor_printHeader(currentExecutor);
    printf("Order: ");
    fflush(stdout);

}

void executor_run(struct Executor *e) {
    // (1) Init the necessary windows
    executor_initLogWindow(e);
    executor_initErrorWindow(e);
    // (2) Executor loop
    char option;
    do {
        cls();
        executor_printHeader(e);
        option = executor_receiveOrder(e);
        switch (option) {
            case 'S': executor_launch(e);
                break;
            case 'I': executor_inform(e);
                break;
            case 'T': executor_terminate(e);
                break;
            case 'E': executor_exit(e);
                break;
        }

    } while (option != 'E');
}

void executor_printHeader(struct Executor *e) {
    printf("*** Command executer ***\n");
    printf("Register: %d commands running; %d terminated\n", e->runningProcesses, e->terminatedProcesses);
    printf("Controls: (S)tart – (I)nform – (T)erminate - (E)xit\n");
}

char executor_receiveOrder(struct Executor *e) {
    char input[MAX_LINE];
    do {
        printf("Order: ");
        gets(input);
    } while (strlen(input) != 1);
    input[0] = toupper(input[0]);
    cls();
    return input[0];
}

void executor_launch(struct Executor *e) {
    char cmd[100]; // the cmd ex: ls -l
    char cmdWithSTDERR[100]; // the cmd with the stderr. ex: ls -l 2> erros.txt
    pid_t pid;

    // (1) get the command
    printf("> ");
    gets(cmd);
    
    // Add stderr redirect to the executor error file
    strcpy(cmdWithSTDERR,cmd);
    strcat(cmdWithSTDERR," 2> ");
    strcat(cmdWithSTDERR,e->errorFileName);
    
    // xterm title
    char xtermTitle[100];
    strcpy(xtermTitle,"\"");
    strcat(xtermTitle,cmd);
    strcat(xtermTitle,"\"");
    
    //(2) fork
    // child
    if ((pid = fork()) == 0) {
        execlp("xterm", "xterm", "-hold", "-T",xtermTitle,"-e", cmdWithSTDERR,NULL);
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
    if (e->runningProcesses == 0)
        printf("There are no running processes!\n");
    else executor_printActiveProcessesWithTimes(e);
    printf("\nPress enter to continue..");
    getchar();
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

void executor_printActiveProcessesWithTimes(struct Executor *e) {
    int totalProcesses = e->processLastIndex + 1;
    int i;
    for (i = 0; i < totalProcesses; i++) {
        if (process_isRunning(e->processes[i])) {
            process_printn(e->processes[i]);
            process_endTime(e->processes[i]);
            process_getUserSystemTimes(e->processes[i]);
            printf("REAL TIME: %fs\n", process_getElapsedTime(e->processes[i]));
            printf("USER TIME: %fs\n", process_getUserTime(e->processes[i]));
            printf("SYSTEM TIME: %fs\n", process_getSysTime(e->processes[i]));
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
    } else {
        printf("There are no processes running!\n");
        printf("\nPress enter to continie..");
        getchar();
    }


}

void executor_exit(struct Executor *e) {
    executor_killAll(e);
    executor_printLogFile(e);
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

    // (2) Append data
    write(logFileDes, log, strlen(log));
    write(logFileDes, "\n", 1);

}

void executor_printError(struct Executor *e, char *error) {

    // (1) Open error file in append mode
    int errorFileDes;
    errorFileDes = open(e->errorFileName, O_RDWR | O_CREAT | O_APPEND, 0777);

    // Error Handling
    if (errorFileDes < 0) {
        perror("errorFile: ");
        exit(1);
    }

    // (2) Append data
    write(errorFileDes, error, strlen(error));
    write(errorFileDes, "\n", 1);


}

void executor_initLogWindow(struct Executor *e) {
    pid_t pid = fork();
    // Child
    if (pid == 0) {
        execlp("xterm", "xterm", "-hold", "-T", "Process Register", "-e", "tail", "-f", e->logFileName, NULL);
    }// Parent
    else {
        e->logWindowPID = pid;
    }
}

void executor_initErrorWindow(struct Executor *e) {
    pid_t pid = fork();
    // Child
    if (pid == 0) {
        execlp("xterm", "xterm", "-hold", "-T", "Error window", "-e", "tail", "-f", e->errorFileName, NULL);
    }// Parent
    else {
        e->errorWindowPID = pid;
    }
}

void executor_printLogFile(struct Executor *e) {

    // (1) Open file
    int logFileDes;
    logFileDes = open(e->logFileName, O_RDONLY);
    if (logFileDes == -1) {
        perror("logfilename:");
    }
    // (2) Read the file
    printf("> Process Register");
    printf("\n--------------------------------------\n");
    char *buff = malloc(sizeof (char*) *100);
    while (read(logFileDes, buff, 1) > 0) {
        printf("%s", buff);
    }
    printf("--------------------------------------\n");
    // (3) Close the file
    close(logFileDes);
}

void executor_killAll(struct Executor *e) {
    int totalProcesses = e->processLastIndex + 1;
    int i;
    // Kill all the processes
    for (i = 0; i < totalProcesses; i++) {
        if (process_isRunning(e->processes[i])) {
            if (kill(process_getPID(e->processes[i]), SIGKILL) < 0) perror("Kill()");
            process_terminate(e->processes[i]);
            //  Add log
            char * buffer = malloc(sizeof (char *) *MAX_LINE);
            double realTime;
            realTime = process_getElapsedTime(e->processes[i]);

            sprintf(buffer, "%s terminated\nRan for: %fs", process_toString(e->processes[i]), realTime);
            executor_addLog(currentExecutor, buffer);

        }
    }

    // Kill the windows
    kill(e->logWindowPID, SIGKILL);
    kill(e->errorWindowPID, SIGKILL);
}