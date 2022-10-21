//
// Created by fer on 14/10/22.
//


#include "p0.h"
struct utsname unameData;


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

        printf("%d\n", getppid()); 

    } else if (tokenNum == 1) {
        printf("%d\n", getpid());
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

                printf("El comando nº %ld no existe en el historial\n", N);

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

    if (tokenNum==1) {
        uname(&unameData);
        printf("%s", unameData.sysname);
        printf("\n%s", unameData.machine);
        printf("\n%s", unameData.nodename);
        printf("\n%s", unameData.release);
        printf("\n%s\n", unameData.version);
    } else {
        printf("Comando no encontrado\n");
    }
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
        } else if (strcmp(tokens[0], "create") == 0) {
            printf("create [-f] [name]\tCrea un directorio o un fichero (-f)\n");
        } else if (strcmp(tokens[0], "stat") == 0) {
            printf("stat [-long][-link][-acc] name1 name2 ..\tlista ficheros;\n"
                   "\t\t-long: listado largo\n"
                   "\t\t-acc: acesstime\n"
                   "\t\t-link: si es enlace simbolico, el path contenido\n");
        } else if (strcmp(tokens[0], "list") == 0) {
            printf("list [-reca] [-recb] [-hid][-long][-link][-acc] n1 n2 ..\tlista contenidos de directorios\n"
                   "\t\t-hid: incluye los ficheros ocultos\n"
                   "\t\t-reca: recursivo (antes)\n"
                   "\t\t-recb: recursivo (despues)\n"
                   "\t\tresto parametros como stat\n");
        } else if (strcmp(tokens[0], "delete") == 0) {
            printf("delete name1 name2 ..\tBorra ficheros o directorios vacios\n");
        } else if (strcmp(tokens[0], "deltree") == 0) {
            printf("deltree name1 name2 ..\tBorra ficheros o directorios no vacios recursivamente\n");
        }
        else{
            printf("Ese comando no existe\n");

        }
    } else if (tokenNum == 1) {
        printf(
                "Lista de comandos:\n autores [-l|-n]\n fecha [-d|-h]\n carpeta [direct]\n"
                " infosis\n comando N\n hist [-c|-N]\n pid [-p]\n fin\n bye\n salir\n create [-f] \n stat[-long][-link][-acc] archivo1 archivo2 ... \n list [-reca] [-recb] [-hid][-long][-link][-acc]\n delete name1 name2 ..\n deltree name1 name2 ..\n");
    } else {
        printf("Comando no encontrado\n");

    }
    return 0;
}
