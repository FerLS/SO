#ifndef PRACTICA1_LISTA_H
#define PRACTICA1_LISTA_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define LNULL NULL

struct struchistData{
    char comando[1024];
    int idCounter;
};
struct structMemData{

    int nBytes;
    void * direccion;
    time_t time;
    char *type;
    char* fichero;
    key_t key;
};

typedef struct struchistData *histData;
typedef struct structMemData *memData;

typedef void *tItemL;
typedef struct Node *tPosL;

struct Node {
    tItemL data;
    tPosL next;
};

typedef tPosL tList;

struct structListas{
    tList listHist;
    tList listMem;
};

typedef struct structListas *Listas;

//Operaciones
void createEmptyList(tList *L);

bool isEmptyList(tList L);

tItemL getItem(tPosL p, tList L /* No es necesaria si se usan punteros*/);

void updateItem(tItemL d, tPosL p, tList *L/* No es necesaria si se usan punteros*/);

tPosL first(tList L);

tPosL previous(tPosL p, tList L);

tPosL next(tPosL p, tList L);

tPosL last(tList L);

tPosL findItem(tList L, bool (*prop)(void *elem, void *), void *extra);

bool insertItem(tItemL d, tPosL p, tList *L);

bool createNode(tPosL *p);

void deleteAtPosition(tPosL p, tList *L);

void deleteList(tList *L);

int sizeList(tList *L);

#endif //PRACTICA1_LISTA_H
