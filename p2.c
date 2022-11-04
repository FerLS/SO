//
// Created by fer on 28/10/22.
//

#include "p2.h"

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

void do_AllocateCreateshared(char *tr[]) {
    key_t cl;
    size_t tam;
    void *p;

    if (tr[0] == NULL || tr[1] == NULL) {
        //ImprimirListaShared(&L);
        return;
    }

    cl = (key_t) strtoul(tr[0], NULL, 10);
    tam = (size_t) strtoul(tr[1], NULL, 10);
    if (tam == 0) {
        printf("No se asignan bloques de 0 bytes\n");
        return;
    }
    if ((p = ObtenerMemoriaShmget(cl, tam)) != NULL)
        printf("Asignados %lu bytes en %p\n", (unsigned long) tam, p);
    else
        printf("Imposible asignar memoria compartida clave %lu:%s\n", (unsigned long) cl, strerror(errno));
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

void do_AllocateMmap(char *arg[]) {
    char *perm;
    void *p;
    int protection = 0;

    if (arg[0] == NULL) {/*ImprimirListaMmap(&L);*/ return; }
    if ((perm = arg[1]) != NULL && strlen(perm) < 4) {
        if (strchr(perm, 'r') != NULL) protection |= PROT_READ;
        if (strchr(perm, 'w') != NULL) protection |= PROT_WRITE;
        if (strchr(perm, 'x') != NULL) protection |= PROT_EXEC;
    }
    if ((p = MapearFichero(arg[0], protection)) == NULL)
        perror("Imposible mapear fichero");
    else
        printf("fichero %s mapeado en %p\n", arg[0], p);
}

void do_DeallocateDelkey(char *args[]) {
    key_t clave;
    int id;
    char *key = args[0];

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


    if (tokenNum == 1) {


    } else if (tokenNum > 1) {

        memData data = NULL;

        if (strcmp(tokens[0], "-malloc") == 0) {

            data= malloc(sizeof(struct structMemData));;
            if(tokenNum == 3){

                data->time = time(NULL);
                data->type = "malloc";
                data->nBytes = atoi(tokens[1]);
                data->direccion = malloc(data->nBytes);
                printf("Asignados %d bytes en %p\n", data->nBytes, data->direccion);

                insertItem(data,NULL,&L->listMem);


            }else{

                printf("******Lista de bloques asignados malloc para el proceso %d\n",getpid());


                for (tPosL p = first(L->listMem); p != last(L->listMem); p = next(p, L->listMem)) { //CUIDADO ESTO no recorre vaica

                    data = (memData)getItem(p,L->listMem);
                    if(strcmp("malloc",data->type) == 0){

                        struct tm tm = *localtime(&data->time);
                        printf("%p\t\t%d-%02d-%02d %02d:%02d:%02d   malloc\n",data->direccion, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

                    }

                }

            }



        }
        else {

            printf("Parametro invalido\n");
        }

    }
    return 0;


}
