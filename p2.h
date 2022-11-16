//
// Created by fer on 28/10/22.
//

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

#endif //PRACTICA1_P2_H
