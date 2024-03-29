//Fernando Alvarez Legisima fernando.alvarezr@udc.es
//Brais Sanchez Ferreiro brais.sferreiro@udc.es

#include "p0.h"
#include "p1.h"
#include "p2.h"
#include "p3.h"



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

    int (*cmd_fun)(char *tokens[], int tokenNum, Listas L);
} cmds[] = {
        {"autores",    autores},
        {"pid",        pid},
        {"carpeta",    carpeta},
        {"fecha",      fecha},
        {"hist",       hist},
        {"comando",    comando},
        {"infosis",    infosis},
        {"ayuda",      ayuda},
        {"create",     create},
        {"deltree",    deltree},
        {"delete",     delete},
        {"stat",       stats},
        {"list",       list},
        {"allocate",   allocate},
        {"deallocate", deallocate},
        {"memdump",    memdump},
        {"memfill",    memfill},
        {"memory",     memory},
        {"recurse",    recursiva},
        {"i-o",        io},
        {"priority",   priority},
        {"showvar",    showvar},
        {"changevar",  changevar},
        {"showenv",    showenv},
        {"fork",       Ourfork},
        {"execute",    execute},
        {"listjobs",   listjobs},
        {"deljobs",    deljobs},
        {"job",        job},
        {NULL, NULL}
};

int process_input(char *tokens[], int tokenNum, Listas L) {
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

    program(tokens,tokenNum,L);
    return 0;
}

void UpdateList(char input[], tList *L) {

    histData item = malloc(sizeof(struct struchistData));
    strcpy(item->comando, input);
    item->idCounter = isEmptyList(*L) ? 0 : ((histData) getItem(last(*L), *L))->idCounter + 1;
    if (!insertItem(item, NULL, L)) {
        printf("No se pudo pibe.");

    }
}

int main(int argc, char *argv[],char *env[]) {

    char input[MAX_INPUT_SIZE];
    char *tokens[MAX_TOKENS];

    int tokenNum;
    tList memList;
    tList histList;
    tList procList;
    createEmptyList(&histList);
    createEmptyList(&memList);
    createEmptyList(&procList);

    Listas listas = malloc(sizeof(struct structListas));

    listas->listHist = histList;
    listas->listMem = memList;
    listas->listProc = procList;

    listas->env = env;
    while (1) {

        printf(YELLOW"> "RESET);

        fgets(input, MAX_INPUT_SIZE, stdin);

        if (input[0] != ' ' && input[0] != '\n') {
            UpdateList(input, &listas->listHist);

            tokenNum = split_string(input, tokens);

            printf("\033[1;34m");
            process_input(tokens, tokenNum, listas);

            if (strcmp(input, "salir") == 0 || strcmp(input, "bye") == 0 || strcmp(input, "fin") == 0) {
                break;
            }
        } else {
            printf("Comando no valido\n");
        }
    }
    deleteList(&listas->listHist);
    FreeListMem(&listas->listMem);
    FreeListProc(&listas->listProc);
    free(listas);
    printf("Bye\n");
}

