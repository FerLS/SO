#include "p0.h"
#include "p3.h"

int priority(char *tokens[], int tokenNum, Listas L){
    int prioridad,pid;
    if(tokenNum!=1){
        pid=atoi(tokens[0]);
        if(tokenNum==2){
            prioridad = getpriority(PRIO_PROCESS,pid);
        }else{
            prioridad = getpid();
            setpriority(PRIO_PROCESS, pid, prioridad);
        }
    }else{
        pid=getpid();
        prioridad= getpriority(PRIO_PROCESS, pid);
    }
    printf("El proceso %d tiene prioridad %d.\n",pid,prioridad);
}

int showvar(char *tokens[], int tokenNum, Listas L){

}

int changevar(char *tokens[], int tokenNum, Listas L){

}

int showenv(char *tokens[], int tokenNum, Listas L){

}

int fork1(char *tokens[], int tokenNum, Listas L){

}

int execute(char *tokens[], int tokenNum, Listas L){

}

int listjobs(char *tokens[], int tokenNum, Listas L){

}

int deljobs(char *tokens[], int tokenNum, Listas L){

}

int job(char *tokens[], int tokenNum, Listas L){

}