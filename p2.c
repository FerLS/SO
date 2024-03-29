//Fernando Alvarez Legisima fernando.alvarezr@udc.es
//Brais Sanchez Ferreiro brais.sferreiro@udc.es

#include "p2.h"
#include "p0.h"

bool comp_dir(void *data, void *extra) {
    return (unsigned long) extra == strtoul((char *) data, NULL, 16);
}

#define TAMANO 2048

typedef struct string string;

void recurse(int n) {
    char automatico[TAMANO];
    static char estatico[TAMANO];

    printf("parametro:%3d(%p) array %p, arr estatico %p\n", n, &n, automatico, estatico);

    if (n > 0)
        recurse(n - 1);
}

void printMemList(char *type, tList *L) {

    memData data;
    bool all = strcmp("all", type) == 0 ? true : false;
    if (isEmptyList(*L)) printf("******No hay bloques asignados para el proceso %d\n", getpid());
    else printf("******Lista de bloques asignados %s para el proceso %d\n", type, getpid());

    tPosL p = first(*L);
    for (int i = 0; i < sizeList(L); ++i) {

        data = (memData) getItem(p, *L);

        if (strcmp(type, data->type) == 0 || all) {


            if (strcmp(data->type, "malloc") == 0) {
                struct tm tm = *localtime(&data->time);
                printf("%p\t\t %d %d-%02d-%02d %02d:%02d:%02d   %s\n", data->direccion, data->nBytes, tm.tm_year + 1900,
                       tm.tm_mon + 1,
                       tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, data->type);
            } else if (strcmp(data->type, "mmap") == 0) {
                struct tm tm = *localtime(&data->time);
                printf("%s\t\t %d-%02d-%02d %02d:%02d:%02d   %s\n", data->fichero, tm.tm_year + 1900,
                       tm.tm_mon + 1,
                       tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, data->type);
            } else if (strcmp(data->type, "shared") == 0) {
                struct tm tm = *localtime(&data->time);
                printf("%p\t\t %d %d-%02d-%02d %02d:%02d:%02d  %s  %d\n", data->direccion, data->nBytes,
                       tm.tm_year + 1900, tm.tm_mon + 1,
                       tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, data->type, data->key);
            }


        }
        p = next(p, *L);
    }
}

void delMemList(char *type, void *x, tList *L) {

    bool all = strcmp("all", type) == 0 ? true : false;
    tPosL p = first(*L);

    for (int i = 0; i < sizeList(L); ++i) {

        memData data = (memData) getItem(p, *L);

        if ((strcmp(data->type, "shared") == 0 && atoi(x) == data->key) || (all && comp_dir(x, data->direccion))) {

            printf("borrado %p\n", data->direccion);

            shmdt(((memData) getItem(p, *L))->direccion);
            deleteAtPosition(p, L);
            break;
        } else if ((strcmp(data->type, "mmap") == 0 && strcmp(data->fichero, (char *) x) == 0) ||
                   (all && comp_dir(x, data->direccion))) {

            printf("borrado %p\n", data->fichero);
            free(data->fichero);
            munmap(data->direccion, (size_t) data->nBytes);
            deleteAtPosition(p, L);
            break;

        } else if ((atoi(x) == data->nBytes) || (all && comp_dir(x, data->direccion))) {

            printf("borrado %p\n", data->direccion);
            free(((memData) getItem(p, *L))->direccion);
            deleteAtPosition(p, L);
            break;
        }
        p = next(p, *L);
    }
}

void LlenarMemoria(void *p, size_t cont, unsigned char byte) {
    unsigned char *arr = (unsigned char *) p;
    size_t i;
    for (i = 0; i < cont; i++)
        arr[i] = byte;
}

void *ObtenerMemoriaShmget(key_t clave, size_t tam, tList *L) {
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


    memData data = malloc(sizeof(struct structMemData));


    data->direccion = p;
    data->nBytes = s.shm_segsz;
    data->key = clave;
    data->type = "shared";

    data->time = time(NULL);
    insertItem(data, NULL, L);


    return (p);
}

void do_AllocateCreateshared(char *tokens[], tList *L) {
    key_t cl;
    size_t tam;
    void *p;

    if (tokens[1] == NULL || tokens[2] == NULL) {
        printMemList("shared", L);
        return;
    }

    cl = (key_t) strtoul(tokens[1], NULL, 10);
    tam = (size_t) strtoul(tokens[2], NULL, 10);
    if (tam == 0) {
        printf("No se asignan bloques de 0 bytes\n");
        return;
    }
    if ((p = ObtenerMemoriaShmget(cl, tam, L)) != NULL) {
        printf("Asignados %s bytes en %p\n", tokens[2], p);
    } else {
        printf("Imposible asignar memoria compartida clave %lu:%s\n", (unsigned long) cl, strerror(errno));
    }
}

void do_AllocateShared(char *tokens[], tList *L) {

    if (tokens[1] == NULL) {
        printMemList("shared", L);
        return;
    }

    key_t cl = (key_t) strtoul(tokens[1], NULL, 10);

    void *p;
    if ((p = ObtenerMemoriaShmget(cl, 0, L)) != NULL) {    //COMO CONSIGUES EL TAMAÑO

        printf("Memoria compartida de clave %d  en %p\n", cl, p);
    } else {
        printf("No se econtro memAsignada con llave %d\n", cl);
    }


}

void *MapearFichero(char *fichero, int protection, char *tokens[], tList *L) {
    int df, map = MAP_PRIVATE, modo = O_RDONLY;
    struct stat s;
    void *p;

    if (protection & PROT_WRITE)
        modo = O_RDWR;
    if (stat(fichero, &s) == -1 || (df = open(fichero, modo)) == -1)
        return NULL;
    if ((p = mmap(NULL, s.st_size, protection, map, df, 0)) == MAP_FAILED)
        return NULL;

    printf("fichero %s mapeado en %p\n", tokens[1], p);
    memData data = malloc(sizeof(struct structMemData));

    data->direccion = p;
    data->type = "mmap";
    data->nBytes = s.st_size;
    data->fichero = strdup(tokens[1]);
    data->time = time(NULL);
    insertItem(data, NULL, L);
    return p;
}

void do_AllocateMmap(char *tokens[], tList *L) {
    char *perm;
    void *p;
    int protection = 0;

    if (tokens[1] == NULL) {
        printMemList("mmap", L);
        return;
    }
    if ((perm = tokens[2]) != NULL && strlen(perm) < 4) {
        if (strchr(perm, 'r') != NULL) protection |= PROT_READ;
        if (strchr(perm, 'w') != NULL) protection |= PROT_WRITE;
        if (strchr(perm, 'x') != NULL) protection |= PROT_EXEC;
    }
    if ((p = MapearFichero(tokens[1], protection, tokens, L)) == NULL) {
        perror("Imposible mapear fichero");
    }


}

void do_AllocateMalloc(char *tokens[], tList *L) {

    if (tokens[1] == NULL) {

        printMemList("malloc", L);
        return;
    }
    if (atoi(tokens[1]) <= 0) {

        printf("Pon un numero mayor que 0\n");
        return;
    } else {

        memData data = malloc(sizeof(struct structMemData));

        data->nBytes = atoi(tokens[1]);
        data->direccion = malloc(data->nBytes);
        data->type = "malloc";
        data->time = time(NULL);
        insertItem(data, NULL, L);
        printf("Asignados %d bytes en %p\n", data->nBytes, data->direccion);

    }
}

void do_DeallocateDelkey(char *tokens[]) {
    key_t clave;
    int id;
    char *key = tokens[1];

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

    printf("Se ha borrado la llave %s.\n", tokens[1]);
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

void do_I_O_read(char *tokens[]) {
    void *p;
    size_t cont = -1;
    ssize_t n;
    if (tokens[1] == NULL || tokens[2] == NULL) {
        printf("faltan parametros\n");
        return;
    }
    p = (void *) strtoul(tokens[2], NULL, 16);
    if (tokens[3] != NULL)
        cont = (size_t) atoll(tokens[3]);

    if ((n = LeerFichero(tokens[1], p, cont)) == -1)
        perror("Imposible leer fichero");
    else
        printf("leidos %lld bytes de %s en %p\n", (long long) n, tokens[1], p);
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

void do_I_O_write(char *tokens[]) {

    void *p;
    size_t cont = -1;
    ssize_t n;

    int o = strcmp(tokens[1], "-o") == 0 ? 1 : 0;

    if (tokens[3 + o] == NULL) {
        printf("faltan parametros\n");
        return;
    }
    p = (void *) strtoul(tokens[2 + o], NULL, 16);
    if (tokens[3] != NULL)
        cont = (size_t) atoll(tokens[3 + o]);

    if ((n = EscribirFichero(tokens[1 + o], p, cont, o)) == -1)
        perror("Imposible escribir fichero");
    else
        printf("escritos %lld bytes en %s desde %p\n", (long long) n, tokens[1 + o], p);


}

void Do_pmap(void) /*sin argumenFtos*/
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
//**************************************************************************************

int allocate(char *tokens[], int tokenNum, Listas L) {

    if (tokenNum > 1) {
        if (strcmp(tokens[0], "-malloc") == 0) {

            do_AllocateMalloc(tokens, &L->listMem);
        } else if (strcmp(tokens[0], "-createshared") == 0) {

            do_AllocateCreateshared(tokens, &L->listMem);
        } else if (strcmp(tokens[0], "-shared") == 0) {

            do_AllocateShared(tokens, &L->listMem);
        } else if (strcmp(tokens[0], "-mmap") == 0) {

            do_AllocateMmap(tokens, &L->listMem);
        } else {

            printf("Parametro invalido\n");
        }
    } else {

        printMemList("all", &L->listMem);
    }
    return 0;
}

int deallocate(char *tokens[], int tokenNum, Listas L) {

    if (tokenNum > 1) {

        if (strcmp(tokens[0], "-malloc") == 0) {

            delMemList("malloc", tokens[1], &L->listMem);

        } else if (strcmp(tokens[0], "-delkey") == 0) {

            do_DeallocateDelkey(tokens);

        } else if (strcmp(tokens[0], "-shared") == 0) {

            delMemList("shared", tokens[1], &L->listMem);

        } else if (strcmp(tokens[0], "-mmap") == 0) {

            delMemList("mmap", tokens[1], &L->listMem);

        } else if (atoi(tokens[0]) > 0) {

            delMemList("all", tokens[1], &L->listMem);

        } else {
            printf("Parametro invalido\n");
        }

    } else {
        printMemList("all", &L->listMem);
    }
    return 0;
}

int memdump(char *tokens[], int tokenNum, Listas L) {
    void *p;
    int n;
    if (tokens[0] != NULL) {


        //si el n que pasas es -1 tiene que leer todo el bloque(no vale usar la lista)
        unsigned long ul = strtoul(tokens[0], NULL, 16);
        if (ul <= 0) return 0;
        p = (void *) ul;
        n = tokens[1] == NULL ? 25 : atoi(tokens[1]);

        unsigned char *arr = (unsigned char *) p;

        if (n < 0) {
            printf("Introduce un numero mayor a 0!\n");
            return 0;
        }

        for (int i = 0; i < n; ++i) {
            printf("%c ", arr[i]);
        }
        printf("\n");
        for (int j = 0; j < n; ++j) {
            printf("%02x ", arr[j]);
        }
        printf("\n");
    }

    return 0;
}

int memfill(char *tokens[], int tokenNum, Listas L) {
    if (tokens[0] != NULL) {
        int tam, c;
        void *p = (void *) strtoull(tokens[0], NULL, 16);


        tam = tokens[1] == NULL ? 128 : atoi(tokens[1]);
        c = tokens[2] == NULL ? 65 : atoi(tokens[2]);

        if (tam < 0 || c < 0) {
            printf("Introduce un numero mayor a 0!\n");
            return 0;
        }
        LlenarMemoria(p, tam, c);
        printf("Llenando %d bytes de memoria con el byte %c(%02X) a partir de la direccion %p\n", tam, c, c, p);
    } else {
        printf("Escriba la direccion, cont y los bytes.\n");
    }
    return 0;
}

int io(char *tokens[], int tokenNum, Listas L) {

    if (tokenNum > 1) {

        if (strcmp("read", tokens[0]) == 0) {
            do_I_O_read(tokens);
        } else if (strcmp("write", tokens[0]) == 0) {
            do_I_O_write(tokens);
        } else {

            printf("Parametro invalido\n");

        }


    } else {
        printf("Faltan parametros\n");
    }
    return 0;
}

int recursiva(char *tokens[], int tokenNum, Listas L) {
    if (tokenNum > 1) {
        recurse(atoi(tokens[0]));
    } else {
        printf("Escribe el número de veces que se va a ejecutar.\n");
    }
    return 0;
}

int global1 = 0, global2 = 0, global3 = 0;

int memory(char *tokens[], int tokenNum, Listas L) {

    int x = 0, y = 0, z = 0;
    static int a = 0, b = 0, c = 0;

    if (tokenNum > 1 && strcmp(tokens[0], "-vars") == 0) {

        printf("Variables locales:\t%p, %p, %p\n", &x, &y, &z);
        printf("Variables estáticas:\t%p, %p, %p\n", &a, &b, &c);
        printf("Variables globales:\t%p, %p, %p\n", &global1, &global2, &global3);
        return 0;

    }
    if (tokenNum > 1 && strcmp(tokens[0], "-funcs") == 0) {

        printf("Funciones de programa:\t%p, %p, %p\n", autores, pid, infosis);
        printf("Funciones de librería:\t%p, %p, %p\n", malloc, printf, strcmp);
        return 0;

    }
    if (tokenNum > 1 && strcmp(tokens[0], "-blocks") == 0) {

        printMemList("all", &L->listMem);
        return 0;

    } else if (tokenNum > 1 && strcmp(tokens[0], "-pmap") == 0) {
        Do_pmap();
        return 0;
    } else {
        printf("Variables locales:\t%p, %p, %p\n", &x, &y, &z);
        printf("Variables estáticas:\t%p, %p, %p\n", &a, &b, &c);
        printf("Variables globales:\t%p, %p, %p\n", &global1, &global2, &global3);
        printf("Funciones de programa:\t%p, %p, %p\n", autores, pid, infosis);
        printf("Funciones de librería:\t%p, %p, %p\n", malloc, printf, strcmp);
        printMemList("all", &L->listMem);
        return 0;

    }
}

void FreeListMem(tList *L) {

    tPosL p = first(*L);

    for (int i = 0; i < sizeList(L); ++i) {

        memData data = (memData) getItem(p, *L);

        if (strcmp(data->type, "shared") == 0) {

            shmdt(((memData) getItem(p, *L))->direccion);
        } else if (strcmp(data->type, "mmap") == 0) {

            free(data->fichero);
            munmap(data->direccion, (size_t) data->nBytes);

        } else {
            free(((memData) getItem(p, *L))->direccion);
        }
        p = next(p, *L);
    }

    deleteList(L);


}
