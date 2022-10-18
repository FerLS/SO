
//Fernando Alvarez Legisima fernando.alvarezr@udc.es
//Brais Sanchez Ferreiro brais.sferreiro@udc.es

#include "p0.h"
#include "p1.h"


int split_string(char *cadena, char *trozos[]) {
    int i = 1;
    if ((trozos[0] = strtok(cadena, " \n\t")) == NULL)
        return 0;
    while ((trozos[i] = strtok(NULL, " \n\t")) != NULL)
        i++;
    return i;

}
struct cmd {
    char *cmd_name;

    int (*cmd_fun)(char *tokens[], int tokenNum, tList *L);
} cmds[] = {
        {"autores", autores},
        {"pid",     pid},
        {"carpeta", carpeta},
        {"fecha",   fecha},
        {"hist",    hist},
        {"comando", comando},
        {"infosis", infosis},
        {"ayuda",   ayuda},
        {"create",  create},
        {"deltree", deltree},
        {"delete",  delete},
        {"stat",    stats},
        {"list",    list},
        {NULL, NULL}
};

int process_input(char *tokens[], int tokenNum, tList *L) {
    int i;

    for (i = 0; cmds[i].cmd_name != NULL; i++) {
        if (strcmp(tokens[0], cmds[i].cmd_name) == 0) {


            cmds[i].cmd_fun(tokens + 1, tokenNum, L);
            return 0;
        }
    }
    if (strcmp(tokens[0], "salir") == 0 || strcmp(tokens[0], "bye") == 0 || strcmp(tokens[0], "fin") == 0) {
        return 0;
    }

    printf("Comando no encontrado\n");
    return 0;
}

void UpdateList(char input[], tList *L) {

    tItemL item;
    strcpy(item.comando, input);
    item.idCounter = isEmptyList(*L) ? 0 : getItem(last(*L), *L).idCounter + 1;
    if (!insertItem(item, NULL, L)) {
        printf("No se pudo pibe.");
    }
}

int main() {
    char input[MAX_INPUT_SIZE];
    char *tokens[MAX_TOKENS];
    int tokenNum = 0;
    tList L;
    createEmptyList(&L);

    while (1) {
        printf("\033[1;33m");
        printf("> ");
        printf(" \033[0;37m");
        fgets(input, MAX_INPUT_SIZE, stdin);

        if (input[0] != ' ' && input[0] != '\n') {
            UpdateList(input, &L);

            tokenNum = split_string(input, tokens);

            printf("\033[1;34m");
            process_input(tokens, tokenNum, &L);

            if (strcmp(input, "salir") == 0 || strcmp(input, "bye") == 0 || strcmp(input, "fin") == 0) {
                break;
            }
        } else {
            printf("Comando no valido\n");

        }

    }
    deleteList(&L);
    printf("Bye\n");


}

