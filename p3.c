//Fernando Alvarez Legisima fernando.alvarezr@udc.es
//Brais Sanchez Ferreiro brais.sferreiro@udc.es

/*
Priority-> getpriority(flag PRIO_PROCESS, int pid);
           setpriority(flag PRIO_PROCESS, int pid, int prioridad); Nice->(-19,19)
-------------------------------------------------------------------------------------
Showvar      -> 1:getenv(char *var);
Showenv           setenv(char *var, char *valor, int overwrite);
Changevar       2:extern char **environ;
                  environ[0] -> "PATH=...."
                3:int main(int argc, char *argv[], char **env);
-------------------------------------------------------------------------------------
Fork -> pid_t fork();
    if(pid<0){
    perror();
    }else if(pid==0){ //Nuevo

    }else{
        waitpid(int pid, NULL, 0); //Espera a que termine pid
    }
-------------------------------------------------------------------------------------
 Execute-> execvpe(args[0], args, environ);->#define _GNU_SOURCE ->Si vuelve, perror.
            char *args[]={"ls","-l",NULL};
           #define _GNU_SOURCE

           ejecutar(char args){
                args[0]=PROGNAME;
-------------------------------------------------------------------------------------
 *****->if((pid=fork())==0){
            ejecutar(...);
            exit(0);
        }else{ //original
            waitpid(pid,NULL,0); // 1er plano
        }
        Probar-> xclock, xterm.
-------------------------------------------------------------------------------------
 listjobs-> int status;
 deljobs    waitpid(int pid, int &status, flag X); WNOHAND   -> No bloqueo
 job                                               WUNTRACED -> Informar stops
                                                   WCONTINUED-> Cuando reanuda
            if(WIFEXITED(status)){
                //Terminó normal
                WEXITEDSTATUS(status);
            }else if(WIFSIGNALED(status)){ //Muerto por señal
                int sig=WTERMSIG(status);

Kill -term pid // kill -stop pid
Kill -kill pid // kill -cont pid
*/

//_----------------------------------------------------------------------------------------------------------------------------------------------------------


/*
 *  Priority
 *
 *      GetPrority(PRIO_PROCESS,pid);
 *      SetPrirority(PRIO_PROCESS,pid,prioridad);   => nice [-19,19]
 *
 *  Showvar
 *
 *      ENV:
 *      char*  getenv(char *var)
 *             setenv(char *var,char * value,int overwrite(a 1))
 *      ENVIRON:
 *      extern char **environ(definir arriba)
 *      environ[0] = "path= ..."
 *      usar la funcion que nos dan
 *
 *      INT MAIN(INT ARGC,CHAR *ARGV[],char **env) usar el tercero
 *
 *  Fork
 *
 *     pid_t pid = fork(); => 2 procesos
 *      if(pid < 0) //ERROR
 *          perror();
 *      else if(pid == 0) //en el nuevo proceso
 *          Nada
 *      else //en el original
 *          WAITPID(pid,NULL,0); //espera a que termine
 *
 *      //si sale bien deberiamos de hacer 2 bye
 *
 *  Execute
 *
 *      char *args[] => {"ls","-l",NULL}
 *      EXECVPE(args[0],args,environ} podemos meter tokens +1
 *      perror()
 *      #define _GNU_SOURCE
 *
 *  ******
 *
 *      if(pid = fork() == 0)
 *          ejecutar();
 *          exit(0)
 *      else
 *
 *          waitpid(pid,null,0) //primer plano
 *
 *  deljob y listjob y job
 *
 *      int status ;
 *      if(pid == waitpid(pid,&status,WNDHANG))    //NO BLOQUO
 *      waitpid(pid,&status,WUNTRACED)    //INFORMAR STOP
 *      waitpid(pid,&status,WCONTINUED)    //CUANDO REANUDA
 *
 *      if(WIFEXITED(status))
 *          // termino normal
 *          WEXIT(status)
 *      else if(WIFSIGNALED(status))
 *          //muerto por señal
 *          int SIG = WTERMSIG(status);
 *
 *      //deljobs
 *
 *          kill -term pid
 *          killl -kill pid
 *
 *          kill -stop pid
 *          kill -cont pid
 *
 *
 * */


#include "p0.h"
#include "p3.h"


void MuestraEntorno(char **entorno, char *nombre_entorno) {
    int i = 0;
    while (entorno[i] != NULL) {
        printf("%p->%s[%d]=(%p) %s\n", &entorno[i],
               nombre_entorno, i, entorno[i], entorno[i]);
        i++;
    }
}

int BuscarVariable (char * var, char *e[]){
    int pos=0;
    char aux[100];
    strcpy (aux,var);
    strcat (aux,"=");
    while (e[pos]!=NULL)
        if (!strncmp(e[pos],aux,strlen(aux)))
            return (pos);
        else
            pos++;
    errno=ENOENT;
    return(-1);
}

int CambiarVariable(char * var, char * valor, char *e[]){
    int pos;
    char *aux;
    if ((pos=BuscarVariable(var,e))==-1)
        return(-1);
    if ((aux=(char *)malloc(strlen(var)+strlen(valor)+2))==NULL)
        return -1;
    strcpy(aux,var); strcat(aux,"=");
    strcat(aux,valor);
    e[pos]=aux;
    return (pos);
}

int priority(char *tokens[], int tokenNum, Listas L) {
    int prioridad, pid;

    if (tokenNum != 1) {
        pid = atoi(tokens[0]);
        if (tokenNum == 2) {
            prioridad = getpriority(PRIO_PROCESS, pid);

        } else {
            prioridad = atoi(tokens[1]);
            if (setpriority(PRIO_PROCESS, pid, prioridad) == -1) {
                perror(RED"No se pudo cambiar la prioridad.\n");
            }
        }
    } else {
        pid = getpid();
        prioridad = getpriority(PRIO_PROCESS, pid);
    }
    printf(BLUE"Prioridad del proceso %d es %d.\n", pid, prioridad);
    return 0;
}


int showvar(char *tokens[], int tokenNum, Listas L) {
    int i;
    if (tokenNum != 0 && tokens[0] != NULL) {
        if ((i = BuscarVariable(tokens[0], __environ)) != -1) {
            printf(MAGENTA"Con arg3 main %s(%p) @%p\n", __environ[i], __environ[i], &__environ[i]);
            printf(MAGENTA"Con environ %s(%p) @%p\n", __environ[i], __environ[i], &__environ[i]);
            printf(MAGENTA"Con getenv %s(%p)\n", getenv(tokens[0]), &__environ[i]);
        } else {
            perror(RED"Error");
        }
    } else {
        MuestraEntorno(__environ, "main");
    }
    return 0;
}

int changevar(char *tokens[], int tokenNum, Listas L) {
    char *aux = malloc(100);
    if (tokenNum != 1) {
        if (tokenNum == 4) {
            if (strcmp(tokens[0], "-a") == 0) {
                CambiarVariable(tokens[1], tokens[2], L->env);
                printf(MAGENTA"Se ha cambiado la variable de entorno %s\n", tokens[1]);
            } else if (strcmp(tokens[0], "-e") == 0) {
                CambiarVariable(tokens[1], tokens[2], __environ);
                printf(MAGENTA"Se ha cambiado la variable de entorno %s\n", tokens[1]);
            } else if (strcmp(tokens[0], "-p") == 0) {
                strcpy(aux, tokens[1]);
                strcat(aux, "=");
                strcat(aux, tokens[2]);
                putenv(aux);
                printf(MAGENTA"Se ha creado la variable de entorno %s\n",tokens[1]);
            }
        }
    } else {
        printf(CYAN"Uso: cambiarvar [-a|-e|-p] var valor\n");
    }
    return 0;
}

int showenv(char *tokens[], int tokenNum, Listas L) {

    if (tokenNum == 1) {

        MuestraEntorno(__environ, "main");

    } else if (tokenNum > 0) {

        if (strcmp(tokens[0], "-addr") == 0) {

            printf(BLUE"environ:   %p (almacenado en %p)\n", __environ, &__environ);
            printf(BLUE"main arg3: %p (almacenado en %p)\n", L->env, &L->env);

        } else if (strcmp(tokens[0], "-environ") == 0) {
            MuestraEntorno(__environ, "main");

        } else {
            printf(RED"Parametros incorrectos\n");
        }
    }
    return 0;
}

int fork1(char *tokens[], int tokenNum, Listas L) {

    pid_t pid = fork();
    if (pid < 0) {
        perror(RED"Error");
    } else if (pid != 0) {
        printf(BLUE"Ejecutando proceso %d\n", pid);
        waitpid(pid, NULL, 0);
    }
    return 0;
}


int execute(char *tokens[], int tokenNum, Listas L) {

    execvpe(tokens[0], tokens, environ);
    perror(RED"Error");
    return 0;
}

int listjobs(char *tokens[], int tokenNum, Listas L) {
    procData data ;
    tPosL p = first(L->listProc);
    for (int i = 0; i < sizeList(&L->listProc); ++i) {
        data = (procData) getItem(p, L->listProc);
        struct tm tm = *localtime(&data->time);
        int status;
        if (waitpid(data->pid,&status, WNOHANG |WUNTRACED |WCONTINUED) == data->pid) {
            if (WIFEXITED(data->out)) {
                strcpy(data->estado, "FINISHED");
                data->out = WEXITSTATUS(data->out);
            } else if (WIFSIGNALED(data->out)) {
                strcpy(data->estado, "SIGNALED");
                data->out = WTERMSIG(data->out);
            } else if (WIFSTOPPED(data->out)) {
                strcpy(data->estado, "STOPPED");
                data->out = WTERMSIG(data->out);
            } else if (WIFCONTINUED(data->out))
                strcpy(data->estado, "ACTIVE");
        }
        printf(CYAN"%d         %s p=%d %d-%02d-%02d %02d:%02d:%02d %s (%03d) %s\n",data->pid, getenv("LOGNAME"),
               getpriority(PRIO_PROCESS,data->pid), tm.tm_year + 1900,tm.tm_mon + 1,
               tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,data->estado,data->out ,data->commandL);

        p = next(p, L->listProc);
    }
    return 0;
}

int deljobs(char *tokens[], int tokenNum, Listas L) {
    if (tokenNum>1) {
        bool term, sig;
        procData data;
        for (int i=0;i<tokenNum-1;i++) {
            if (strcmp(tokens[i],"-term") == 0) {
                term = true;
            }
            if (strcmp(tokens[i],"-sig") == 0) {
                sig = true;
            }
        }
        tPosL p;
        do{
            for(p=first(L->listProc); !end(L->listProc, p); p=next(p, L->listProc)){
                data= getItem(p,L->listProc);
                if(sig && WIFSIGNALED(data->out)){
                    printf(MAGENTA"Se ha borrado el proceso %d\n",data->pid);
                    deleteAtPosition(p,&L->listProc);
                    break;
                }
                else {
                    if (term && WIFEXITED(data->out)) {
                        printf(MAGENTA"Se ha borrado el proceso %d\n",data->pid);
                        deleteAtPosition(p, &L->listProc);
                        break;
                    }
                }
            }
        }while(!end(L->listProc,p));
        listjobs(tokens, tokenNum, L);
    } else {
        listjobs(tokens, tokenNum, L);
    }
    return 0;
}

int job(char *tokens[], int tokenNum, Listas L) {
    procData data;
    if(tokenNum!=1){
        tPosL p= first(L->listProc);
        for(int i=0; i< sizeList(&L->listProc);i++) {
            data = (procData) getItem(p, L->listProc);
            struct tm tm = *localtime(&data->time);
            if (strcmp(tokens[0], "-fg") == 0) {
                if (data->pid == atoi(tokens[1])) {
                    waitpid(data->pid, NULL, 0);
                    if (strcmp(data->estado, "ACTIVE") == 0) {
                        printf(BLUE"Proceso %d terminado normalmente. Valor devuelto %d\n", data->pid, data->out);
                    } else {
                        printf(RED"Proceso %d pid ya esta finalizado\n", data->pid);
                    }
                    deleteAtPosition(p, &L->listProc);
                    break;
                }
            }else {
                if (tokenNum == 2) {
                    printf(CYAN"%d         %s p=%d %d-%02d-%02d %02d:%02d:%02d %s (%03d) %s\n", data->pid,
                           getenv("LOGNAME"), getpriority(PRIO_PROCESS, data->pid), tm.tm_year + 1900,
                           tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,
                           data->estado, data->out, data->commandL);
                }
                break;
            }
            p = next(p, L->listProc);
        }
    }else{
        listjobs(tokens,tokenNum,L);
    }
    return 0;
}

int program(char *tokens[], int tokenNum, Listas L) {
    pid_t pid;
    bool secondPlan;
    tokenNum = tokenNum < 0 ? 0 : tokenNum;
    if(tokenNum > 1){
        secondPlan = strcmp(tokens[tokenNum-1],"&") == 0;
        if(secondPlan){
            tokens[tokenNum-1] =  0;
            tokenNum--;
        }
    }else{
        secondPlan = false;
    }

    if ((pid = fork()) == 0) {
        execute(tokens, tokenNum, L);
        exit(0);
    } else if(!secondPlan){
        waitpid(pid, NULL, 0);  //PRIMER PLANO
    }

    if(!secondPlan) return 0;
    //SEGUNDO PLANO(se supone que solo mete los de segundo plano)
    procData data = malloc(sizeof(struct structProcData));

    data->time = time(NULL);
    char *command = malloc(MAX_INPUT_SIZE * MAX_TOKENS);

    for (int i = 0; i < tokenNum; ++i) {
        strcat(command,tokens[i]);
        strcat(command," ");
    }
    data->commandL = command;

    data->pid = pid;
    data->priority = setpriority(PRIO_PROCESS,pid,0);
    strcpy(data->estado,"ACTIVE");

    insertItem(data, NULL, &L->listProc);


    return 0;
}
