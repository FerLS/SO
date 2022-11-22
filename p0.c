//
// Created by fer on 14/10/22.
//


#include "p0.h"
struct utsname unameData;


int autores(char *tokens[], int tokenNum, Listas L) {

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

int pid(char *tokens[], int tokenNum, Listas L) {

    if (tokenNum == 2 && strcmp(tokens[0], "-p") == 0) {

        printf("%d\n", getppid()); 

    } else if (tokenNum == 1) {
        printf("%d\n", getpid());
    } else {
        printf("Comando no encontrado\n");

    }
    return 0;

}

int fecha(char *tokens[], int tokenNum, Listas L) {

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

int hist(char *tokens[], int tokenNum, Listas L) {
    int cont = 0;
    char *text;
    if (tokenNum == 2 && strcmp(tokens[0], "-c") == 0) {
        deleteList(&L->listHist);
    } else if (tokenNum == 2) {

        long N = strtol(tokens[0] + 1, &text, 10);

        if (tokens[0][0] == '-' && N > 0) {

            for (tPosL p = first(L->listHist); p != last(L->listHist) && cont < N; p = next(p, L->listHist)) {

                printf("%d -> %s", cont, ((histData)getItem(p, L->listHist))->comando);
                cont++;

            }


        } else {
            printf("Comando no encontrado\n");

        }

    } else if (tokenNum == 1) {

        for (tPosL p = first(L->listHist); p != last(L->listHist); p = next(p, L->listHist)) {

            printf("%d -> %s", ((histData)getItem(p, L->listHist))->idCounter, ((histData) getItem(p, L->listHist))->comando);

        }


    } else {

        printf("Comando no encontrado\n");

    }

    return 0;
}

int comando(char *tokens[], int tokenNum,Listas L) {
    char *text;


    if (tokenNum == 2) {

        if (!isEmptyList(L->listHist)) {
            long N = strtol(tokens[0], &text, 10);


            tPosL p;
            for (p = first(L->listHist); p != last(L->listHist) && ((histData) getItem(p, L->listHist))->idCounter != N; p = next(p, L->listHist));

            if (N == ((histData)getItem(p, L->listHist))->idCounter) {
                char comando[20];
                strcpy(comando, ((histData)getItem(p, L->listHist))->comando);
                split_string(comando, tokens);

                if (strcmp(tokens[0], "comando") == 0) {

                    printf("No se puede ejecutar comando desde el mismo comando!\n");

                } else {
                    char ch = '\n';
                    UpdateList(strncat(comando, &ch, 1), &(L->listHist));
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

int infosis(char *tokens[], int tokenNum, Listas L) {

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

int ayuda(char *tokens[], int tokenNum, Listas L) {

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
        }else if(strcmp(tokens[0],"allocate")==0){
            printf("allocate [-malloc|-shared|-createshared|-mmap]... Asigna un bloque de memoria\n"
                   "\t\t-malloc tam: asigna un bloque malloc de tamano tam\n"
                   "\t\t-createshared cl tam: asigna (creando) el bloque de memoria compartida de clave cl y tamano tam\n"
                   "\t\t-shared cl: asigna el bloque de memoria compartida (ya existente) de clave cl\n"
                   "\t\t-mmap fich perm: mapea el fichero fich, perm son los permisos\n");
        }else if(strcmp(tokens[0],"deallocate")==0){
            printf("deallocate [-malloc|-shared|-delkey|-mmap|addr]..\tDesasigna un bloque de memoria\n"
                   "\t\t-malloc tam: desasigna el bloque malloc de tamano tam\n"
                   "\t\t-shared cl: desasigna (desmapea) el bloque de memoria compartida de clave cl\n"
                   "\t\t-delkey cl: elimina del sistema (sin desmapear) la clave de memoria cl\n"
                   "\t\t-mmap fich: desmapea el fichero mapeado fich\n"
                   "\t\taddr: desasigna el bloque de memoria en la direccion addr\n");
        }else if(strcmp(tokens[0],"i-o")==0){
            printf("i-o [read|write] [-o] fiche addr cont\n"
                   "\tread fich addr cont: Lee cont bytes desde fich a addr\n"
                   "\twrite [-o] fich addr cont: Escribe cont bytes desde addr a fich. -o para sobreescribir\n"
                   "\t\taddr es una direccion de memoria\n");
        }else if(strcmp(tokens[0],"memdump")==0){
            printf("memdump addr cont 	Vuelca en pantallas los contenidos (cont bytes) de la posicion de memoria addr\n");
        }else if(strcmp(tokens[0],"memfill")==0){
            printf("memfill addr cont byte \tLlena la memoria a partir de addr con byte\n");
        }else if(strcmp(tokens[0],"memory")==0){
            printf("memory [-blocks|-funcs|-vars|-all|-pmap] ..\tMuestra muestra detalles de la memoria del proceso\n"
                   "\t\t-blocks: los bloques de memoria asignados\n"
                   "\t\t-funcs: las direcciones de las funciones\n"
                   "\t\t-vars: las direcciones de las variables\n"
                   "\t\t-all: todo\n"
                   "\t\t-pmap: muestra la salida del comando pmap(o similar)\n");
        }else if(strcmp(tokens[0],"recurse")==0){
            printf("recurse [n]\tInvoca a la funcion recursiva n veces\n");
        }else if(strcmp(tokens[0],"priority")==0){
            printf("priority [pid] [valor] \tMuestra o cambia la prioridad del proceso pid a valor\n");
        }else if(strcmp(tokens[0],"showvar")==0){
            printf("showvar var1\tMuestra el valor y las direcciones de la variable de entorno var\n");
        }else if(strcmp(tokens[0],"changevar")==0){
            printf("changevar [-a|-e|-p] var valor\tCambia el valor de una variable de entorno\n"
                   "\t-a: accede por el tercer arg de main\n"
                   "\t-e: accede mediante environ\n"
                   "\t-p: accede mediante putenv\n");
        }else if(strcmp(tokens[0],"showenv")==0){
            printf("showenv [-environ|-addr] \t Muestra el entorno del proceso\n");
        }else if(strcmp(tokens[0],"fork")==0){
            printf("fork \tHace una llamada fork para crear un proceso\n");
        }else if(strcmp(tokens[0],"execute")==0){
            printf("execute prog args....\tEjecuta, sin crear proceso,prog con argumentos\n");
        }else if(strcmp(tokens[0],"listjobs")==0){
            printf("listjobs \tLista los procesos en segundo plano\n");
        }else if(strcmp(tokens[0],"deljobs")==0){
            printf("deljobs [-term][-sig]\tElimina los procesos de la lista procesos en sp\n"
                   "\t-term: los terminados\n"
                   "\t-sig: los terminados por senal");
        }else if(strcmp(tokens[0],"job")==0){
            printf("job [-fg] pid\tMuestra informacion del proceso pid.\n"
                   "\t-fg: lo pasa a primer plano");
        }
        else{
            printf("Ese comando no existe\n");
        }
    } else if (tokenNum == 1) {
        printf(
                " Lista de comandos:\n autores [-l|-n]\n fecha [-d|-h]\n carpeta [direct]\n"
                " infosis\n comando N\n hist [-c|-N]\n pid [-p]\n fin\n bye\n salir\n"
                " create [-f] \n stat[-long][-link][-acc] archivo1 archivo2 ... \n list [-reca] [-recb] [-hid][-long][-link][-acc]\n "
                "delete name1 name2 ..\n deltree name1 name2 ..\n"
                " allocate [-malloc|-shared|-createshared|-mmap]\n deallocate [-malloc|-shared|-delkey|-mmap|addr]\n i-o [read|write] [-o]\n"
                " memdump\n memfill\n memory\n recurse\n priority\n showvar\n changevar\n showenv\n fork\n execute\n listjobs\n"
                " deljobs\n job\n *****");
    } else {
        printf("Comando no encontrado\n");

    }
    return 0;
}
