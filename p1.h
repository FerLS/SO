//
// Created by fer on 14/10/22.
//

#ifndef PRACTICA1_P1_H
#define PRACTICA1_P1_H
#include <cabezeras.h>
#define MAX_PATH 1000
struct utsname unameData;

char LetraTF(mode_t m);
char *ConvierteModo(mode_t m) ;

bool get_item(char *path, struct stat *st) ;
int stat_item(char *path , struct stat *st, bool lonng, bool acc, bool link);

int delete_item(char *path, bool recursive);

int list_item(char *path, bool lonng, bool acc, bool link, bool reca, bool recb, bool hid) ;
int carpeta(char *tokens[], int tokenNum, tList *L);
int create(char *tokens[], int tokenNum, tList *L) ;

int deltree(char *tokens[], int tokenNum, tList *L);

int delete(char *tokens[], int tokenNum, tList *L) ;

int stats(char *tokens[], int tokenNum, tList *L) ;

int list(char *tokens[], int tokenNum, tList *L);
#endif //PRACTICA1_P1_H
