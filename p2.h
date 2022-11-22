//Fernando Alvarez Legisima fernando.alvarezr@udc.es
//Brais Sanchez Ferreiro brais.sferreiro@udc.es

#ifndef PRACTICA1_P2_H
#define PRACTICA1_P2_H

#include "headers.h"

int allocate(char *tokens[], int tokenNum, Listas L);

int deallocate(char *tokens[], int tokenNum, Listas L);

int memdump(char *tokens[], int tokenNum, Listas L);

int memfill(char *tokens[], int tokenNum, Listas L);

int memory(char *tokens[], int tokenNum, Listas L);

int recursiva(char *tokens[], int tokenNum, Listas L);

int io(char *tokens[], int tokenNum, Listas L);

void FreeListMem(tList *L);

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

#endif //PRACTICA1_P2_H
