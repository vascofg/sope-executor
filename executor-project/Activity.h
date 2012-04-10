#ifndef ACTIVITY_H
#define	ACTIVITY_H

#include "Process.h"
/** 
 * Activity.h - A struct to describe a determinated activity that an user did in 
 * the executor. This activity can be the launching or the termination of a process.
 * So a process can have associated 2 activities: one for launching and other for killing
 */
struct Activity {
    char type;
    bool success;
    struct Process *process;
};
/**
 * Creates a new activity with the necessary information
 * @param a the pointer of the activity
 * @param type the type of the activity
 * @param success the sucess boolean value
 * @param process the pointer of the process associated
 */
void activity_create(struct Activity *a, char type, bool success, struct Process *process);

// Set methods
void activity_setType(struct Activity *a, char type);
void activity_setSucess(struct Activity *a, bool success);
void activity_setProcess(struct Activity *a,struct Process *process );

// Get methods
char activity_getType(struct Activity *a);
bool activity_getSuccess(struct Activity *a);
struct Process * activity_getProcess(struct Activity *a);

// Others methods
void activity_print(struct Activity *a);
void activity_printn(struct Activity *a);


#endif

