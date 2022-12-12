//Fernando Alvarez Legisima fernando.alvarezr@udc.es
//Brais Sanchez Ferreiro brais.sferreiro@udc.es

#ifndef PRACTICA1_HEADERS_H
#define PRACTICA1_HEADERS_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/utsname.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <pwd.h>
#include <sys/shm.h>
#include "lista.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/resource.h>
#endif //PRACTICA1_HEADERS_H

#define RESET   "\x1b[0m"
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define MAX_INPUT_SIZE 1024
#define MAX_TOKENS 64

int split_string(char *cadena, char *trozos[]);

int process_input(char *tokens[], int tokenNum, Listas L);

void UpdateList(char input[], tList *L);