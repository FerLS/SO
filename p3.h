#ifndef PRACTICA1_P3_H
#define PRACTICA1_P3_H

#include "headers.h"

int priority(char *tokens[], int tokenNum, Listas L);

int showvar(char *tokens[], int tokenNum, Listas L);

int changevar(char *tokens[], int tokenNum, Listas L);

int showenv(char *tokens[], int tokenNum, Listas L);

int fork1(char *tokens[], int tokenNum, Listas L);

int execute(char *tokens[], int tokenNum, Listas L);

int listjobs(char *tokens[], int tokenNum, Listas L);

int deljobs(char *tokens[], int tokenNum, Listas L);

int job(char *tokens[], int tokenNum, Listas L);

#endif //PRACTICA1_P3_H
