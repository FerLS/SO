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

int BuscarVariable (char * var, char *e[])  /*busca una variable en el entorno que se le pasa como parámetro*/{
    int pos=0;
    char aux[20];

    strcpy (aux,var);
    strcat (aux,"=");

    while (e[pos]!=NULL)
        if (!strncmp(e[pos],aux,strlen(aux)))
            return (pos);
        else
            pos++;
    errno=ENOENT;   /*no hay tal variable*/
    return(-1);
}

void MuestraEntorno (char **entorno, char * nombre_entorno){
    int i=0;
    while (entorno[i]!=NULL){
        printf ("%p->%s[%d]=(%p) %s\n", &entorno[i],
                nombre_entorno, i,entorno[i],entorno[i]);
        i++;
    }
}

int showvar(char *tokens[], int tokenNum, Listas L){
    int pos;
    if(tokenNum!=0 && tokens[0]!=NULL){
        if((pos=BuscarVariable(tokens[0],__environ))!=-1){
            printf(MAGENTA"Con arg3 main %s(%p) @%p\n",__environ[pos],__environ[pos],&L->listProc->data);
            printf("Con environ %s(%p) @%p\n",__environ[pos],__environ[pos],&__environ[pos]);
            printf("Con getenv %s(%p)\n",getenv(tokens[0]),&__environ[pos]);
        }else{
            perror("Error");
        }
    }else{
        MuestraEntorno(__environ,"main");
    }
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