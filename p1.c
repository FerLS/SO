
//Fernando Alvarez Legisima fernando.alvarezr@udc.es
//Brais Sanchez Ferreiro brais.sferreiro@udc.es

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<unistd.h>
#include <errno.h>
#include <time.h>
#include "lista.h"
#include <sys/utsname.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <fcntl.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKENS 64
#define MAX_PATH 1000
struct utsname unameData;

int process_input(char *tokens[], int tokenNum, tList *L);

void UpdateList(char input[], tList *L);

char LetraTF (mode_t m)
{
    switch (m&S_IFMT) { /*and bit a bit con los bits de formato,0170000 */
        case S_IFSOCK: return 's'; /*socket */
        case S_IFLNK: return 'l'; /*symbolic link*/
        case S_IFREG: return '-'; /* fichero normal*/
        case S_IFBLK: return 'b'; /*block device*/
        case S_IFDIR: return 'd'; /*directorio */
        case S_IFCHR: return 'c'; /*char device*/
        case S_IFIFO: return 'p'; /*pipe*/
        default: return '?'; /*desconocido, no deberia aparecer*/
    }
}
/*las siguientes funciones devuelven los permisos de un fichero en formato rwx----*/
/*a partir del campo st_mode de la estructura stat */
/*las tres son correctas pero usan distintas estrategias de asignación de memoria*/

char * ConvierteModo (mode_t m, char *permisos)
{
    strcpy (permisos,"---------- ");

    permisos[0]=LetraTF(m);
    if (m&S_IRUSR) permisos[1]='r';    /*propietario*/
    if (m&S_IWUSR) permisos[2]='w';
    if (m&S_IXUSR) permisos[3]='x';
    if (m&S_IRGRP) permisos[4]='r';    /*grupo*/
    if (m&S_IWGRP) permisos[5]='w';
    if (m&S_IXGRP) permisos[6]='x';
    if (m&S_IROTH) permisos[7]='r';    /*resto*/
    if (m&S_IWOTH) permisos[8]='w';
    if (m&S_IXOTH) permisos[9]='x';
    if (m&S_ISUID) permisos[3]='s';    /*setuid, setgid y stickybit*/
    if (m&S_ISGID) permisos[6]='s';
    if (m&S_ISVTX) permisos[9]='t';

    return permisos;
}

int split_string(char *cadena, char *trozos[]) {
    int i = 1;
    if ((trozos[0] = strtok(cadena, " \n\t")) == NULL)
        return 0;
    while ((trozos[i] = strtok(NULL, " \n\t")) != NULL)
        i++;
    return i;

}

int autores(char *tokens[], int tokenNum, tList *L) {

    if (tokenNum == 2 && strcmp(tokens[0], "-l") == 0) {
        printf("brais.sferreiro@udc.es\n");
        printf(" fernando.alvarezr@udc.es\n");
    } else if (tokenNum == 2 && strcmp(tokens[0], "-n") == 0) {
        printf("Brais Sanchez Ferreiro\n");
        printf(" Fernando Alvarez Rodriguez de Legisima\n");
    } else if (tokenNum == 1) {
        printf("Fernando Alvarez Legisima\n fernando.alvarezr@udc.es\n");
        printf(" Brais Sanchez Ferreiro\n brais.sferreiro@udc.es\n");
    } else {
        printf("Comando no encontrado\n");

    }
    return 0;
}

int pid(char *tokens[], int tokenNum, tList *L) {

    if (tokenNum == 2 && strcmp(tokens[0], "-p") == 0) {

        printf("%d\n", getpid()); //getppdid;

    } else if (tokenNum == 1) {
        printf("%d\n", getpid());
    } else {
        printf("Comando no encontrado\n");

    }
    return 0;

}

int carpeta(char *tokens[], int tokenNum, tList *L) {

    char path[100];

    if (tokenNum == 2) {

        if (chdir(tokens[0]) == 0) {
            printf("Directorio cambiado a : %s\n", tokens[0]);
        } else {
            printf("Error: %s\n", strerror(errno));
        }
    } else if (tokenNum == 1) {

        getcwd(path, 100);
        printf("Directorio actual : %s\n", path);
    } else {
        printf("Comando no encontrado\n");

    }
    return 0;

}

int fecha(char *tokens[], int tokenNum, tList *L) {

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    if (tokenNum == 2 && strcmp(tokens[0], "-d") == 0) {

        printf("%d-%02d-%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);


    } else if (tokenNum == 2 && strcmp(tokens[0], "-h") == 0) {

        printf("%02d:%02d:%02d\n", tm.tm_hour, tm.tm_min, tm.tm_sec);

    } else if (tokenNum == 1) {


        printf("%d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min,
               tm.tm_sec);

    } else {
        printf("Comando no encontrado\n");

    }
    return 0;

}

int hist(char *tokens[], int tokenNum, tList *L) {
    int cont = 0;
    char *text;
    if (tokenNum == 2 && strcmp(tokens[0], "-c") == 0) {
        deleteList(L);
    } else if (tokenNum == 2) {

        long N = strtol(tokens[0] + 1, &text, 10);

        if (tokens[0][0] == '-' && N > 0) {

            for (tPosL p = first(*L); p != last(*L) && cont < N; p = next(p, *L)) {

                printf("%d -> %s", cont, getItem(p, *L).comando);
                cont++;

            }


        } else {
            printf("Comando no encontrado\n");

        }

    } else if (tokenNum == 1) {

        for (tPosL p = first(*L); p != last(*L); p = next(p, *L)) {

            printf("%d -> %s", getItem(p, *L).idCounter, getItem(p, *L).comando);

        }


    } else {

        printf("Comando no encontrado\n");

    }

    return 0;
}

int comando(char *tokens[], int tokenNum, tList *L) {
    char *text;


    if (tokenNum == 2) {

        if (!isEmptyList(*L)) {
            long N = strtol(tokens[0], &text, 10);


            tPosL p;
            for (p = first(*L); p != last(*L) && getItem(p, *L).idCounter != N; p = next(p, *L));

            if (N == getItem(p, *L).idCounter) {
                char comando[20];
                strcpy(comando, getItem(p, *L).comando);
                split_string(comando, tokens);

                if (strcmp(tokens[0], "comando") == 0) {

                    printf("No se puede ejecutar comando desde el mismo comando!\n");

                } else {
                    char ch = '\n';
                    UpdateList(strncat(comando, &ch, 1), L);
                    tokenNum = split_string(comando, tokens);
                    process_input(tokens, tokenNum, L);

                }
            } else {

                printf("El comando nº %ld no existe en el historial\n",N);

            }


        } else {
            printf("No hay comandos previos\n");

        }


    } else if (tokenNum == 1) {
        printf("Escriba un numero de comando\n");
    } else {
        printf("Comando de comando no encontrado\n");

    }

    return 0;
}

int infosis(char *tokens[], int tokenNum, tList *L) {

    uname(&unameData);
    printf("%s", unameData.sysname);
    printf("\n%s", unameData.machine);
    printf("\n%s", unameData.nodename);
    printf("\n%s", unameData.release);
    printf("\n%s\n", unameData.version);
    return 0;
}

int ayuda(char *tokens[], int tokenNum, tList *L) {

    if (tokenNum == 2) {

        if (strcmp(tokens[0], "autores") == 0) {
            printf("autores [-l|-n] : Imprime los nombres e inicios de sesion de los autores del programa. autores -l imprime\n"
                   " solo los inicios de sesion y los autores -n imprime solo los nombres.\n\n");
        } else if (strcmp(tokens[0], "fecha") == 0) {
            printf("fecha [-d|-h] : Sin argumentos imprime tanto la fecha actual como el tiempo.\n"
                   " actual. fecha -d imprime la fecha actual.\n"
                   " fecha -h imprime la hora actual.\n\n");
        } else if (strcmp(tokens[0], "carpeta") == 0) {
            printf("carpeta [direct] : Cambia el directorio de trabajo actual del shell a directo . Cuando se invoca sin augurios imprime"
                   "  el directorio de trabajo actual.\n\n");
        } else if (strcmp(tokens[0], "infosis") == 0) {
            printf("infosis : Imprime informacion de la maquina.\n\n");
        } else if (strcmp(tokens[0], "comando") == 0) {
            printf("comando N Repite el comando numero N (de la lista historica).\n\n");
        } else if (strcmp(tokens[0], "hist") == 0) {
            printf("hist [-c|-N] : Muestra/borra el historial de comandos ejecutados por este shell.\n"
                   " hist -c borra el historico, es decir, vacia la lista\n\n"
                   "\t+ hist Imprime todos los comandos que se han ingresado con su orden.\n"
                   "\t+ hist -c Borra (vacia) la lista de comandos historicos.\n"
                   "\t+ hist -N Imprime los primeros N comandos.\n\n");
        } else if (strcmp(tokens[0], "pid") == 0) {
            printf("pid [-p] : Imprime el pid del proceso que ejecuta el shell. pid -p el pid\n"
                   " del proceso padre del shell.\n\n");
        } else if (strcmp(tokens[0], "fin") == 0 || strcmp(tokens[0], "bye") == 0 || strcmp(tokens[0], "salir") == 0) {
            printf("bye/salir/end Termina con la shell.\n");
        } else {
            printf("Comando no encontrado\n");
        }
    } else if (tokenNum == 1) {
        printf(
                "Lista de comandos:\n autores [-l|-n]\n fecha [-d|-h]\n carpeta [direct]\n"
                " infosis\n comando N\n hist [-c|-N]\n pid [-p]\n fin\n bye\n salir\n\n");
    } else {
        printf("Comando no encontrado\n");

    }
    return 0;
}

int create(char *tokens[], int tokenNum, tList *L) {


    if(tokenNum == 3 && strcmp(tokens[0], "-f") == 0){

        int fd = open(tokens[1],O_CREAT,S_IWUSR | S_IRUSR);
        if(fd < 0){
            printf("Error: %s\n", strerror(errno));


        }
        close(fd);

    }
    else if(tokenNum == 2){

        if (mkdir(tokens[0],S_IRWXU )<0){

            printf("Error: %s\n", strerror(errno));

        }

    }else{

        printf("Comando no encontrado\n");
    }

}

struct cmd {
    char *cmd_name;

    int (*cmd_fun)(char *tokens[], int tokenNum, tList *L);
} cmds[] = {
        {"autores", autores},
        {"pid",     pid},
        {"carpeta", carpeta},
        {"fecha",   fecha},
        {"hist",    hist},
        {"comando", comando},
        {"infosis", infosis},
        {"ayuda",   ayuda},
        {"create",   create},
        {NULL, NULL}
};

int process_input(char *tokens[], int tokenNum, tList *L) {
    int i;

    for (i = 0; cmds[i].cmd_name != NULL; i++) {
        if (strcmp(tokens[0], cmds[i].cmd_name) == 0) {


            cmds[i].cmd_fun(tokens + 1, tokenNum, L);
            return 0;
        }
    }
    if (strcmp(tokens[0], "salir") == 0 || strcmp(tokens[0], "bye") == 0 || strcmp(tokens[0], "fin") == 0) {
        return 0;
    }

    printf("Comando no encontrado\n");
    return 0;
}

void UpdateList(char input[], tList *L) {

    tItemL item;
    strcpy(item.comando, input);
    item.idCounter = isEmptyList(*L) ? 0 : getItem(last(*L), *L).idCounter + 1;
    if (!insertItem(item, NULL, L)) {
        printf("No se pudo pibe.");
    }
}

int main() {
    char input[MAX_INPUT_SIZE];
    char *tokens[MAX_TOKENS];
    int tokenNum = 0;
    tList L;
    createEmptyList(&L);

    while (1) {
        printf("\033[1;33m");
        printf("> ");
        printf(" \033[0;37m");
        fgets(input, MAX_INPUT_SIZE, stdin);

        if (input[0] != ' ' && input[0] != '\n') {
            UpdateList(input, &L);

            tokenNum = split_string(input, tokens);

            printf("\033[1;34m");
            process_input(tokens, tokenNum, &L);

            if (strcmp(input, "salir") == 0 || strcmp(input, "bye") == 0 || strcmp(input, "fin") == 0) {
                break;
            }
        } else {
            printf("Comando no valido\n");

        }

    }
    deleteList(&L);
    printf("Bye\n");



}

int delete_item(char *path){


    struct stat st;
    if( lstat(path,&st) == -1){

        printf("Could no acces %n", strerror((errno)));

        return  0;
    }
    if((st.st_mode & S_IFMT) == S_IFDIR){ //ES UN DIRECTORIO
        DIR *d;
        struct dirent *ent;
        if((d == opendir(path))){

            printf("Could no open %n", strerror((errno)));
            return  0;
        }
        while ((ent = readdir(d))!= NULL){

            char new_path[MAX_PATH];


            if(strcmp(ent->d_name,".") == 0 || strcmp(ent->d_name,"..") == 0){
                continue;
            }

            sprintf(new_path,"%s%s",path,ent->d_name);
            delete_item(delete_item(ent->d_name));
        }
        close(d);

    }
    /*
    if(remove(path) == -1){
        printf("Could no de lete %n", strerror((errno)));

    }
     */

    printf("Borrar %s\n",path);
}

int delTree(int nargs,char *tokens[]){

    for (int i = 1; i != NULL; ++i) {

        delete_item(tokens[i]);

    }
}