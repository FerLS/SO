//
// Created by fer on 18/10/22.
//

#ifndef PRACTICA1_HEADERS_H
#define PRACTICA1_HEADERS_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/utsname.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <fcntl.h>
#include <pwd.h>
#include "lista.h"
#endif //PRACTICA1_HEADERS_H

#define MAX_INPUT_SIZE 1024
#define MAX_TOKENS 64


int split_string(char *cadena, char *trozos[]);

int process_input(char *tokens[], int tokenNum, tList *L);

void UpdateList(char input[], tList *L);