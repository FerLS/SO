//
// Created by fer on 14/10/22.
//

#ifndef PRACTICA1_P1_H
#define PRACTICA1_P1_H
#include "headers.h"

#define MAX_PATH 1000


struct statParms{

    bool lonng;
    bool link;
    bool acc;

    bool reca;
    bool recb;
    bool hid;

};



char LetraTF(mode_t m);
char *ConvierteModo(mode_t m) ;

bool get_item(char *path, struct stat *st) ;
int stat_item(char *path , struct stat *st, struct  statParms *stP);

int delete_item(char *path, bool recursive);

int list_item(char *path, struct statParms *stP) ;
int carpeta(char *tokens[], int tokenNum,Listas L);
int create(char *tokens[], int tokenNum,Listas L) ;

int deltree(char *tokens[], int tokenNum,Listas L);

int delete(char *tokens[], int tokenNum,Listas L) ;

int stats(char *tokens[], int tokenNum,Listas L) ;

int list(char *tokens[], int tokenNum,Listas L);
#endif //PRACTICA1_P1_H
