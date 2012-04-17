#include <stdio.h>
#include <stdlib.h>
#include "Process.h"
#include "Executor.h"
#include <signal.h>

int main(int argc, char* argv[]) {
    struct Executor e;   
    signal(SIGCHLD, executor_sigchildHandler);
    executor_create(&e,"register.txt","errors.txt");
    executor_run(&e);
    exit(0);
}