#include <stdio.h>
#include <stdlib.h>
#include "Process.h"
#include "Activity.h"
#include "Executor.h"
int main(int argc, char* argv[]) {
    
    /*
     /// Test process
    struct Process ls;
    process_create(&ls,1,"ls -laR /",13446);
    process_printn(&ls);
    
    // Test activity
    struct Activity launch_ls;
    activity_create(&launch_ls,'l',1,&ls);
    activity_printn(&launch_ls);
    */
    struct Executor e;
    executor_create(&e,"registos.txt","erros.txt");
    executor_run(&e);
    
    exit(0);
}

