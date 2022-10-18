
#ifndef PRACTICA1_LISTA_H
#define PRACTICA1_LISTA_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define LNULL NULL

struct datos{
    char comando[20];
    int idCounter;
};

typedef struct datos tItemL;
typedef struct Node *tPosL;

struct Node {

    tItemL data;
    tPosL next;

};


typedef tPosL tList;

//Operaciones

void createEmptyList(tList *L);

bool isEmptyList(tList L);

tItemL getItem(tPosL p, tList L /* No es necesaria si se usan punteros*/);

void updateItem(tItemL d, tPosL p, tList *L/* No es necesaria si se usan punteros*/);

tPosL first(tList L);

tPosL previous(tPosL p, tList L);

tPosL next(tPosL p, tList L);

tPosL last(tList L);

tPosL findItem(tItemL d, tList L);

bool insertItem(tItemL d, tPosL p, tList *L);

bool createNode(tPosL *p);

void deleteAtPosition(tPosL p, tList *L);

void deleteList(tList *L);

#endif //PRACTICA1_LISTA_H
