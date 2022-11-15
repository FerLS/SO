//
// Created by fer on 28/10/22.
//

#include "p2.h"
#include "p0.h"

/*
// ALLOCATE

 //-malloc
lo de siempre
//-share && createshared

 1 - int ID =  shm get(clave(atoi para hacela int),tamaño de la zona compartida,IPC_CREAT(create share,porque la estamos creando),
 2 - void *PTR SHM AT(ID,NULL,flags para solo lectura o tal (0 para lectura y escritura) //Puntero a la zona compartida

 // -mmap

 0 - int FD = OPEN(...);
 1 - mmap(NULL(elegie el kernel),parte de longitud del fichero que queremos ver,flags para leer o escribir y tal,flags para otras cosas(MAP_SHARED),FD,offest donde empezar a leer);




//DEALLOCATE


 * necsitamos guarda en una estrucura los allocate
 convertir lista a generica (puntero a void);
 crear estructira de bloque(guardar tipo de allocate para despues)
    Time_T Tim = TIME(NULL);

    //-malloc
    free
    //shared

    SHMDT(void *K)

    //-mmap
    munmap(void *,length)

    //Direccion de memoria

    buscar en lista y liberarla segun TIPO,
    tenemos que convertir el string de direccion de memeoria a puntero
    a un unsignedlong
    void *ptr = (void*) STRTOUL(string,NULL(donde eempieza otra cosa que no es el numero),16(base hexadecimal);

//I-O

 converitmos como antes y leemos o escribimos

//MEMDUMP

 Imprime los n bytes de un direccion de memoria
 printf como caracter y numero


//MEMFILL

 escribit un caracter(en codigo ascci) en una posicion de memoria n veces

//MEMORY

 //-blocks
 imprimir infomacion de la lista de allocate

 //-funcs

 printf("%p",AUTORES); //elegimos 3 de liberia y 3 nuestras
 Las direccion cambian siempre por seguridad

 lirbreira > programe

 //-vars

 Imprime la direccion de 3 vairbales globales y tres estaticas (no usamos las globales xd)

 locales > globales == estatica en direccion de memotria


 //-all

 tod o lo de antes

 //-pmap

 imirpime la memoria al reves por alguna razon xd

 asi es normal

 kernel
 stack

 break(muchas cosas)

 heap
 data
 codigo

 a bueno que ya esta hecho menos mal xd

//RECURSE

 imprime recursivemente direacicones de  memoria del parametro que pasamos

 1 - delcaramos un array y un aaray estaticco, hacemos llamadas recurisvas




*/

struct meminfo {
    int size;
};

bool comp_size(void *data, void *extra) {
    struct meminfo *inf = data;
    int *size = extra;
    return inf->size == *size;
}

int size = 1000;
//find(comp_size, &size);

#define TAMANO 2048
void printMemList(char *type, tList *L) {

    memData data;
    bool all = strcmp("all", type) == 0 ? true : false;
    if (isEmptyList(*L)) printf("******No hay bloques asignados para el proceso %d\n", getpid());
    else printf("******Lista de bloques asignados %s para el proceso %d\n", type, getpid());

    tPosL p = first(*L);
    for (int i = 0; i < sizeList(L); ++i) {

        data = (memData) getItem(p, *L);

        if (strcmp(type, data->type) == 0 || all) {


            if(strcmp(type,"malloc") == 0 || strcmp(type,"shared") == 0){
                struct tm tm = *localtime(&data->time);
                printf("%p\t\t%d-%02d-%02d %02d:%02d:%02d   %s\n", data->direccion, tm.tm_year + 1900, tm.tm_mon + 1,
                       tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, data->type);
            }
            if(strcmp(type,"mmap") == 0){
                struct tm tm = *localtime(&data->time);
                printf("%s\t\t%d-%02d-%02d %02d:%02d:%02d   %s\n", data->fichero, tm.tm_year + 1900, tm.tm_mon + 1,
                       tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, data->type);
            }


        }
        p = next(p, *L);
    }


}
void Recursiva(int n) {
    char automatico[TAMANO];
    static char estatico[TAMANO];

    printf("parametro:%3d(%p) array %p, arr estatico %p\n", n, &n, automatico, estatico);

    if (n > 0)
        Recursiva(n - 1);
}


void LlenarMemoria(void *p, size_t cont, unsigned char byte) {
    unsigned char *arr = (unsigned char *) p;
    size_t i;

    for (i = 0; i < cont; i++)
        arr[i] = byte;
}

void *ObtenerMemoriaShmget(key_t clave, size_t tam) {
    void *p;
    int aux, id, flags = 0777;
    struct shmid_ds s;

    if (tam)     /*tam distito de 0 indica crear */
        flags = flags | IPC_CREAT | IPC_EXCL;
    if (clave == IPC_PRIVATE)  /*no nos vale*/
    {
        errno = EINVAL;
        return NULL;
    }
    if ((id = shmget(clave, tam, flags)) == -1)
        return (NULL);
    if ((p = shmat(id, NULL, 0)) == (void *) -1) {
        aux = errno;
        if (tam)
            shmctl(id, IPC_RMID, NULL);
        errno = aux;
        return (NULL);
    }
    shmctl(id, IPC_STAT, &s);
    /* Guardar en la lista   InsertarNodoShared (&L, p, s.shm_segsz, clave); */
    return (p);
}

void do_AllocateCreateshared(char *tokens[], tList *L) {
    key_t cl;
    size_t tam;
    void *p;

    if (tokens[1] == NULL || tokens[2] == NULL) {
        printMemList("createshared", L);
        return;
    }

    cl = (key_t) strtoul(tokens[1], NULL, 10);
    tam = (size_t) strtoul(tokens[2], NULL, 10);
    if (tam == 0) {
        printf("No se asignan bloques de 0 bytes\n");
        return;
    }
    if ((p = ObtenerMemoriaShmget(cl, tam)) != NULL){
        printf("Asignados %lu bytes en %p\n", (unsigned long) tam, p);
        memData data = malloc(sizeof(struct structMemData));

        data->direccion = p;
        data->nBytes = atoi(tokens[2]);
        data->type = "shared";

        data->time = time(NULL);
        insertItem(data, NULL, L);

    }
    else{
        printf("Imposible asignar memoria compartida clave %lu:%s\n", (unsigned long) cl, strerror(errno));
    }
}


void *MapearFichero(char *fichero, int protection) {
    int df, map = MAP_PRIVATE, modo = O_RDONLY;
    struct stat s;
    void *p;

    if (protection & PROT_WRITE)
        modo = O_RDWR;
    if (stat(fichero, &s) == -1 || (df = open(fichero, modo)) == -1)
        return NULL;
    if ((p = mmap(NULL, s.st_size, protection, map, df, 0)) == MAP_FAILED)
        return NULL;
/* Guardar en la lista    InsertarNodoMmap (&L,p, s.st_size,df,fichero); */
    return p;
}

void do_AllocateMmap(char *tokens[],tList *L) {
    char *perm;
    void *p;
    int protection = 0;

    if (tokens[1] == NULL) { printMemList("mmap",L); return; }
    if ((perm = tokens[2]) != NULL && strlen(perm) < 4) {
        if (strchr(perm, 'r') != NULL) protection |= PROT_READ;
        if (strchr(perm, 'w') != NULL) protection |= PROT_WRITE;
        if (strchr(perm, 'x') != NULL) protection |= PROT_EXEC;
    }
    if ((p = MapearFichero(tokens[1], protection)) == NULL)
        perror("Imposible mapear fichero");
    else{

        printf("fichero %s mapeado en %p\n", tokens[1], p);
        memData data = malloc(sizeof(struct structMemData));

        data->direccion = p;
        data->type = "mmap";

        data->fichero = tokens[1];
        data->time = time(NULL);
        insertItem(data, NULL, L);
    }



}
void do_AllocateMalloc(char *tokens[],tList *L) {

    if (atoi(tokens[1]) <= 0) {

        printf("Pon un numero mayor que 0\n");
        return ;
    }else{

        memData data = malloc(sizeof(struct structMemData));

        data->direccion =malloc(atoi(tokens[1]));
        data->fichero = tokens[1];
        data->type = "malloc";
        data->time = time(NULL);
        insertItem(data, NULL, L);
        printf("Asignados %d bytes en %p\n",data->nBytes , data->direccion);

    }



}

void do_DeallocateDelkey(char *tokens[]) {
    key_t clave;
    int id;
    char *key = tokens[0];

    if (key == NULL || (clave = (key_t) strtoul(key, NULL, 10)) == IPC_PRIVATE) {
        printf("      delkey necesita clave_valida\n");
        return;
    }
    if ((id = shmget(clave, 0, 0666)) == -1) {
        perror("shmget: imposible obtener memoria compartida");
        return;
    }
    if (shmctl(id, IPC_RMID, NULL) == -1)
        perror("shmctl: imposible eliminar memoria compartida\n");
}


ssize_t LeerFichero(char *f, void *p, size_t cont) {
    struct stat s;
    ssize_t n;
    int df, aux;

    if (stat(f, &s) == -1 || (df = open(f, O_RDONLY)) == -1)
        return -1;
    if (cont == -1)   /* si pasamos -1 como bytes a leer lo leemos entero*/
        cont = s.st_size;
    if ((n = read(df, p, cont)) == -1) {
        aux = errno;
        close(df);
        errno = aux;
        return -1;
    }
    close(df);
    return n;
}

void do_I_O_read(char *ar[]) {
    void *p;
    size_t cont = -1;
    ssize_t n;
    if (ar[0] == NULL || ar[1] == NULL) {
        printf("faltan parametros\n");
        return;
    }
    //p=/cadtop(ar[1]);  /*convertimos de cadena a puntero*/
    if (ar[2] != NULL)
        cont = (size_t) atoll(ar[2]);

    if ((n = LeerFichero(ar[0], p, cont)) == -1)
        perror("Imposible leer fichero");
    else
        printf("leidos %lld bytes de %s en %p\n", (long long) n, ar[0], p);
}

ssize_t EscribirFichero(char *f, void *p, size_t cont, int overwrite) {
    ssize_t n;
    int df, aux, flags = O_CREAT | O_EXCL | O_WRONLY;

    if (overwrite)
        flags = O_CREAT | O_WRONLY | O_TRUNC;

    if ((df = open(f, flags, 0777)) == -1)
        return -1;

    if ((n = write(df, p, cont)) == -1) {
        aux = errno;
        close(df);
        errno = aux;
        return -1;
    }
    close(df);
    return n;
}


void Do_pmap(void) /*sin argumentos*/
{
    pid_t pid;       /*hace el pmap (o equivalente) del proceso actual*/
    char elpid[32];
    char *argv[4] = {"pmap", elpid, NULL};

    sprintf(elpid, "%d", (int) getpid());
    if ((pid = fork()) == -1) {
        perror("Imposible crear proceso");
        return;
    }
    if (pid == 0) {
        if (execvp(argv[0], argv) == -1)
            perror("cannot execute pmap (linux, solaris)");

        argv[0] = "procstat";
        argv[1] = "vm";
        argv[2] = elpid;
        argv[3] = NULL;
        if (execvp(argv[0], argv) == -1)/*No hay pmap, probamos procstat FreeBSD */
            perror("cannot execute procstat (FreeBSD)");

        argv[0] = "procmap", argv[1] = elpid;
        argv[2] = NULL;
        if (execvp(argv[0], argv) == -1)  /*probamos procmap OpenBSD*/
            perror("cannot execute procmap (OpenBSD)");

        argv[0] = "vmmap";
        argv[1] = "-interleave";
        argv[2] = elpid;
        argv[3] = NULL;
        if (execvp(argv[0], argv) == -1) /*probamos vmmap Mac-OS*/
            perror("cannot execute vmmap (Mac-OS)");
        exit(1);
    }
    waitpid(pid, NULL, 0);
}




int allocate(char *tokens[], int tokenNum, Listas L) {


    if (tokenNum > 1) {


        if (strcmp(tokens[0], "-malloc") == 0) {


            if (tokenNum == 3) {

                do_AllocateMalloc(tokens, &L->listMem);


            } else {

                printMemList("malloc", &L->listMem);

            }

        }
        else if(strcmp(tokens[0], "-createshared") == 0){

            if(tokenNum == 3){


                do_AllocateCreateshared(tokens,&L->listMem);

            }

        }
        else if(strcmp(tokens[0], "-shared") == 0){


        }
        else if(strcmp(tokens[0], "-mmap") == 0){

            if(tokenNum == 3){



                do_AllocateMmap(tokens,&L->listMem);

            }

        }
        else {

            printf("Parametro invalido\n");
        }

    } else {
        printMemList("all", &L->listMem);

    }
    return 0;


}

void memdump(char *tokens[], int tokenNum, Listas L){
    void *p;

    if(tokens[0]!=NULL){

        p= (void *) strtoul(tokens[0],NULL,16);

        if(tokens[2]==NULL){

            for (int i = 0; i < 25; ++i) {
                printf("%c ",*(char *)(p+1));
            }
            printf("\n");
            for (int j = 0; j < 25; ++j) {
                printf("%2x ",*(char *)(p+1));
            }
            printf("\n");
        }else{
            for (int i = 0; i < atoi(tokens[1]); ++i) {
                printf("%c ",*(char *)(p+1));
            }
            printf("\n");
            for (int j = 0; j < atoi(tokens[1]); ++j) {
                printf("%2x ",*(char *)(p+1));
            }
            printf("\n");
        }
    }
}

void memfill(char *tokens[], int tokenNum, Listas L){
    int i;
    void *p;

    if (tokens[0] != NULL){

        p = (void *)strtoull(tokens[0], NULL, 16);
        if (tokens[1] == NULL){
            for (i = 0; i < 128; i++) {
                (*(char *) (p + i)) = 65;
            }
        }
        if (tokens[2] == NULL){
            for (i = 0; i < atoi(tokens[1]); i++) {
                (*(char *) (p + i)) = 65;
            }
        }
        for (i = 0; i < atoi(tokens[1]); i++) {
            (*(char *) (p + i)) = tokens[2][0];
        }
    }
}
/*
//global variables for function memoria
int global1=0,global2=0,global3=0;
//Muestra muestra detalles de la memoria del proceso
int memory(char *tokens[], int tokenNum, Listas *L){
    if(tokenNum !=0){
        for(int i=0;i<tokenNum;i++){
            if(strcmp(tokens[i], "-vars")== 0){
                auto int x=0,y=0,z=0;
                static int a=0,b=0,c=0;

                printf("automatic variables:\t%p, %p, %p\n", &x, &y, &z);
                printf("static variables:\t%p, %p, %p\n", &a, &b, &c);
                printf("global variables:\t%p, %p, %p\n", &global1, &global2, &global3);

            }if(strcmp(tokens[i], "-funcs")== 0){
                printf("program functions:\t%p, %p, %p\n", autores, pid, infosis);
                printf("library functions:\t%p, %p, %p\n", malloc, printf, strcmp);

            }if(strcmp(tokens[i], "-blocks")== 0){


            }else if(strcmp(tokens[i], "-all")== 0){
                char *input[] = {"-vars","-funcs", "-blocks"};
                memory(input, 3, L);

            }else if(strcmp(tokens[i], "-pmap")== 0){

            }
        }
    }else{
        char *input[] = {"-all"};
        memory(input, 1, L);
    }
    return 0;
}
*/