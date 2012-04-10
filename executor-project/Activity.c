#include "Activity.h"
#include "Process.h"
#include <stdbool.h>

void activity_create(struct Activity *a, char type, bool success, struct Process *process){
    a->type=type;
    a->success=success;
    a->process=process;
}

void activity_setType(struct Activity *a, char type){
    a->type=type;
}

void activity_setSucess(struct Activity *a, bool success){
    a->success=success;
}

void activity_setProcess(struct Activity *a,struct Process *process ){
    a->process=process;
}

char activity_getType(struct Activity *a){
    return a->type;
}

bool activity_getSuccess(struct Activity *a){
    return a->success;
}

struct Process * activity_getProcess(struct Activity *a){
    return a->process;
}
void activity_print(struct Activity *a){
    // Type of activity
    if(a->type=='l') printf("Launched ");
    else if(a->type=='t') printf("Terminated ");
    
    //The command and PID
    printf("\"%s\" ",process_getCommand(a->process));
    printf("with PID %d ",process_getPID(a->process));
    
    // The sucess
    if(a->success==true) printf("sucessfully.");
    else printf("failed.");
    
}

void activity_printn(struct Activity *a){
    activity_print(a);
    printf("\n");
}