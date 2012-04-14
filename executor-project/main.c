#include <stdio.h>
#include <stdlib.h>
#include "Process.h"
#include "Executor.h"

int main(int argc, char* argv[]) {
    struct Executor e;   
    executor_create(&e,"registos.txt","erros.txt");
    executor_run(&e);
    exit(0);
}

