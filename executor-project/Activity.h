#ifndef ACTIVITY_H
#define	ACTIVITY_H

struct Activity{
    char type;
    char *command;
    bool success;
    int PID;
};

void Activity(struct Activity a, char type, char *command,bool success,int pid );

void setType(struct Activity a, char type);
void setCommand(struct Activity a, char *command);
void setSucess(struct Activity a, bool success);
void setPID(struct Activity a, int PID );

char getType(struct Activity a);
char * getCommand(struct Activity a);
bool getSuccess(struct Activity a);
bool getPID(struct Activity a);

void printActivity(struct Activity a);


#endif

