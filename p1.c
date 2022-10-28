//
// Created by fer on 14/10/22.
//

#include "p1.h"


char LetraTF(mode_t m) {
    switch (m & S_IFMT) { /*and bit a bit con los bits de formato,0170000 */
        case S_IFSOCK:
            return 's'; /*socket */
        case S_IFLNK:
            return 'l'; /*symbolic link*/
        case S_IFREG:
            return '-'; /* fichero normal*/
        case S_IFBLK:
            return 'b'; /*block device*/
        case S_IFDIR:
            return 'd'; /*directorio */
        case S_IFCHR:
            return 'c'; /*char device*/
        case S_IFIFO:
            return 'p'; /*pipe*/
        default:
            return '?'; /*desconocido, no deberia aparecer*/
    }
}

char *ConvierteModo(mode_t m) {
    static char permisos[12];
    strcpy(permisos, "---------- ");

    permisos[0] = LetraTF(m);
    if (m & S_IRUSR) permisos[1] = 'r';    /*propietario*/
    if (m & S_IWUSR) permisos[2] = 'w';
    if (m & S_IXUSR) permisos[3] = 'x';
    if (m & S_IRGRP) permisos[4] = 'r';    /*grupo*/
    if (m & S_IWGRP) permisos[5] = 'w';
    if (m & S_IXGRP) permisos[6] = 'x';
    if (m & S_IROTH) permisos[7] = 'r';    /*resto*/
    if (m & S_IWOTH) permisos[8] = 'w';
    if (m & S_IXOTH) permisos[9] = 'x';
    if (m & S_ISUID) permisos[3] = 's';    /*setuid, setgid y stickybit*/
    if (m & S_ISGID) permisos[6] = 's';
    if (m & S_ISVTX) permisos[9] = 't';

    return permisos;
}

bool get_item(char *path, struct stat *st) {

    if (lstat(path, st) == -1) {

        printf("No se pudo acceder a %s :  %s\n", path, strerror((errno)));

        return false;
    }


    return true;
}

int stat_item(char *path, struct stat *st, struct statParms *stP) {

    struct tm *time;

    if (stP->lonng) {

        time = localtime(&st->st_mtime);
        printf("%d/%d/%d-%d:%d\t %lu(%ld)\t%s\t%s %s \t", time->tm_year + 1900, time->tm_mon, time->tm_mday,
               time->tm_hour, time->tm_min,
               st->st_nlink, st->st_ino, getpwuid(st->st_uid)->pw_name, getpwuid(st->st_gid)->pw_name,
               ConvierteModo(st->st_mode));

        if (stP->acc) {

            time = localtime(&st->st_atime);
            printf("%d/%d/%d-%d:%d\t", time->tm_year + 1900, time->tm_mon, time->tm_mday, time->tm_hour, time->tm_min);
        }
        if (stP->link) {


            ssize_t buffSize, nbytes;
            buffSize = st->st_size + 1;
            char *buf;

            buf = NULL;
            buf = malloc(buffSize);

            nbytes = readlink(path, buf, buffSize);
            if (nbytes != -1) {

                buf[st->st_size] = '\0';
                printf("\t%ld  %s", st->st_size, path);
                printf(" -> %s\n", buf);
            }
            free(buf);
        }

    }

    if(!stP->link) printf("\t%ld  %s\n", st->st_size, path);

    return 0;

}

int delete_item(char *path, bool recursive) {

    struct stat st;

    if (!get_item(path, &st)) return 0;

    if ((st.st_mode & S_IFMT) == S_IFDIR) { //ES UN DIRECTORIO
        DIR *d;
        struct dirent *ent;
        if ((d = opendir(path)) == NULL) {

            printf("No se pudo abrir %s :  %s \n", path, strerror((errno)));
            return 0;
        }

        if (recursive) {
            while ((ent = readdir(d)) != NULL) {

                char new_path[MAX_PATH];


                if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
                    continue;
                }

                sprintf(new_path, "%s/%s", path, ent->d_name);
                delete_item(new_path, true);
            }
        }

        closedir(d);

    }

    if (remove(path) == -1) {
        printf("No se pudo borrar %s : %s\n", path, strerror((errno)));

    } else {
        printf("Borrar %s\n", path);

    }
    return 0;

}


int list_item(char *path, struct statParms *stP) {

    struct stat st;

    if (!get_item(path, &st)) return 0;

    if (stP->recb && stP->reca) printf("No se puede iterar de dos maneras a la vez! \n");


    if ((st.st_mode & S_IFMT) == S_IFDIR) { //ES UN DIRECTORIO

        DIR *d;
        struct dirent *ent;

        if ((d = opendir(path)) == NULL) {

            printf("No se pudo abrir %s :  %s \n", path, strerror((errno)));
            return 0;
        }

        if (!stP->recb) {

            printf("************%s\n", path);

            while ((ent = readdir(d)) != NULL) {

                char new_path[MAX_PATH];

                if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
                    continue;
                }
                if (ent->d_name[0] == '.' && !stP->hid)continue;
                sprintf(new_path, "%s/%s", path, ent->d_name);

                get_item(new_path, &st);
                stat_item(ent->d_name, &st, stP);

            }
            closedir(d);
            d = opendir(path);
        }

        while ((ent = readdir(d)) != NULL) {

            char new_path[MAX_PATH];

            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
                continue;
            }
            if (ent->d_name[0] == '.' && !stP->hid)continue;
            sprintf(new_path, "%s/%s", path, ent->d_name);


            get_item(new_path, &st);
            if (stP->reca || stP->recb) list_item(new_path, stP);

        }

        if (stP->recb) {

            printf("************%s\n", path);

            closedir(d);
            d = opendir(path);


            while ((ent = readdir(d)) != NULL) {

                char new_path[MAX_PATH];

                if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
                    continue;
                }
                if (ent->d_name[0] == '.' && !stP->hid)continue;
                sprintf(new_path, "%s/%s", path, ent->d_name);

                get_item(new_path, &st);
                stat_item(ent->d_name, &st, stP);

            }


        }

        closedir(d);


    }


    return 0;


}


int carpeta(char *tokens[], int tokenNum, tList *L) {

    char path[100];

    if (tokenNum == 2) {

        if (chdir(tokens[0]) == 0) {
            printf("Directorio cambiado a : %s\n", tokens[0]);
        } else {
            printf("Error: %s\n", strerror(errno));
        }
    } else if (tokenNum == 1) {

        getcwd(path, 100);
        printf("Directorio actual : %s\n", path);
    } else {
        printf("Comando no encontrado\n");

    }
    return 0;

}


int create(char *tokens[], int tokenNum, tList *L) {


    if (tokenNum == 3 && strcmp(tokens[0], "-f") == 0) {

        int fd = open(tokens[1], O_CREAT, S_IWUSR | S_IRUSR);
        if (fd < 0) {
            printf("Error: %s\n", strerror(errno));


        } else {
            close(fd);

        }


    } else if (tokenNum == 2) {

        if (strcmp(tokens[0], "-f") == 0) {
            printf("Escriba un nombre del fichero.\n");
        } else if (mkdir(tokens[0], S_IRWXU) < 0) {

            printf("Error: %s\n", strerror(errno));

        }

    } else {

        printf("Escriba un nombre de la carpeta\n");
    }
    return 0;


}

int deltree(char *tokens[], int tokenNum, tList *L) {

    if (tokenNum >= 2) {

        for (int i = 0; i < tokenNum - 1; ++i) {

            delete_item(tokens[i], true);

        }
    } else {
        printf("Ponga algo que borrar.\n");
    }
    return 0;


}

int delete(char *tokens[], int tokenNum, tList *L) {

    if (tokenNum >= 2) {

        for (int i = 0; i < tokenNum - 1; ++i) {


            delete_item(tokens[i], false);

        }
    } else {
        printf("Ponga algo que borrar.\n");
    }
    return 0;

}

int stats(char *tokens[], int tokenNum, tList *L) {

    struct stat st;
    struct statParms stP;
    int counter = 0;

    stP.link = false;
    stP.lonng = false;
    stP.acc = false;
    stP.hid = false;
    stP.recb = false;
    stP.reca = false;


    for (int i = 0; i < tokenNum - 1; ++i) {

        if (strcmp(tokens[i], "-long") == 0) {
            stP.lonng = true;
            counter++;
        } else if (strcmp(tokens[i], "-acc") == 0) {
            stP.acc = true;
            counter++;
        } else if (strcmp(tokens[i], "-link") == 0) {
            stP.link = true;
            counter++;

        }

    }
    if (tokenNum == 1) {
        printf("Comando no encontrado\n");
        return 0;

    }
    if (counter == tokenNum - 1) {

        printf("Introduzca un nombre de fichero\n");
        return 0;
    }

    for (int i = counter; i < tokenNum - 1; ++i) {


        if (!get_item(tokens[i], &st)) return 0;

        stat_item(tokens[i], &st, &stP);
    }
    return 0;


}

int list(char *tokens[], int tokenNum, tList *L) {


    int counter = 0;
    struct statParms stP;
    stP.link = false;
    stP.lonng = false;
    stP.acc = false;
    stP.hid = false;
    stP.recb = false;
    stP.reca = false;

    for (int i = 0; i < tokenNum - 1; ++i) {

        if (strcmp(tokens[i], "-long") == 0) {
            stP.lonng = true;
            counter++;
        } else if (strcmp(tokens[i], "-acc") == 0) {
            stP.acc = true;
            counter++;
        } else if (strcmp(tokens[i], "-link") == 0) {
            stP.link = true;
            counter++;

        } else if (strcmp(tokens[i], "-reca") == 0) {

            stP.reca = true;
            counter++;

        } else if (strcmp(tokens[i], "-recb") == 0) {

            stP.recb = true;
            counter++;

        } else if (strcmp(tokens[i], "-hid") == 0) {

            stP.hid = true;
            counter++;

        }
    }


    if (tokenNum == 1) {
        printf("Comando no encontrado\n");
        return 0;
    }

    for (int i = counter; i < tokenNum - 1; ++i) {


        list_item(tokens[i], &stP);


    }
    return 0;

}

