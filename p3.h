//Fernando Alvarez Legisima fernando.alvarezr@udc.es
//Brais Sanchez Ferreiro brais.sferreiro@udc.es

#ifndef PRACTICA1_P3_H
#define PRACTICA1_P3_H

#include "headers.h"

int priority(char *tokens[], int tokenNum, Listas L);

int showvar(char *tokens[], int tokenNum, Listas L);

int changevar(char *tokens[], int tokenNum, Listas L);

int showenv(char *tokens[], int tokenNum, Listas L);

int Ourfork(char *tokens[], int tokenNum, Listas L);

int execute(char *tokens[], int tokenNum, Listas L);

int listjobs(char *tokens[], int tokenNum, Listas L);

int deljobs(char *tokens[], int tokenNum, Listas L);

int job(char *tokens[], int tokenNum, Listas L);

int program(char *tokens[], int tokenNum, Listas L);

void FreeListProc(tList *L);

bool isNumber(char *string);

#endif //PRACTICA1_P3_H
