//Fernando Alvarez Legisima fernando.alvarezr@udc.es
//Brais Sanchez Ferreiro brais.sferreiro@udc.es

#include "p0.h"
#include "p3.h"

int priority(char *tokens[], int tokenNum, Listas L){
    int prioridad,pid;
    if(tokenNum!=1){
        pid=atoi(tokens[0]);
        if(tokenNum==2){
            prioridad = getpriority(PRIO_PROCESS,pid);

        }else{
            prioridad = atoi(tokens[1]);
            if(setpriority(PRIO_PROCESS, pid, prioridad)==-1){
                perror("No se pudo cambiar la prioridad.\n");
            }
        }
    }else{
        pid=getpid();
        prioridad= getpriority(PRIO_PROCESS, pid);
    }
    printf("Prioridad del proceso %d es %d.\n",pid,prioridad);
    return 0;
}

int showvar(char *tokens[], int tokenNum, Listas L){
    int pos;

    return 0;
}

int changevar(char *tokens[], int tokenNum, Listas L){

    return 0;
}

int showenv(char *tokens[], int tokenNum, Listas L){

    return 0;
}

int fork1(char *tokens[], int tokenNum, Listas L){

    return 0;
}

int execute(char *tokens[], int tokenNum, Listas L){

    return 0;
}

int listjobs(char *tokens[], int tokenNum, Listas L){

    return 0;
}

int deljobs(char *tokens[], int tokenNum, Listas L){

    return 0;
}

int job(char *tokens[], int tokenNum, Listas L){

    return 0;
}