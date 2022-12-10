#include "lista.h"

void createEmptyList(tList *L) {

    *L = LNULL;
}

bool isEmptyList(tList L) {

    return L == LNULL;

}

tItemL getItem(tPosL p, tList L /* No es necesaria si se usan punteros*/) {

    return (p->data);

}

void updateItem(tItemL d, tPosL p, tList *L /* No es necesaria si se usan punteros*/) {

    p->data = d;

}

tPosL first(tList L) {

    return L;
}

tPosL previous(tPosL p, tList L) {

    if (p == L) return LNULL;
    else {

        tPosL q;
        for (q = L; p != q->next; q = q->next);
        return q;

    }
}

tPosL next(tPosL p, tList L) {

    return p->next;
}

tPosL last(tList L) {

    tPosL p;
    for (p = L; p->next != LNULL; p = p->next);
    return p;

}

int end(tList L, tPosL p) { // estoy en el final
    return (p==NULL);
}

tPosL findItem(tList L, bool (*prop)(void *elem, void *), void *extra) {

    tPosL p;
    if (isEmptyList(L)) {

        p = LNULL;
    } else {

        for (p = L; p != LNULL && !prop(p->data,
                                        extra); p = p->next); //Esto esta mal ya que el identificador es igual, habria que inventarse in id
    }
    return p;

}

bool insertItem(tItemL d, tPosL p, tList *L) {

    tPosL q;
    if (!createNode(&q)) {

        return false;

    } else {

        q->data = d;
        q->next = LNULL; //por si acaso
    }
    if (isEmptyList(*L)) {

        *L = q;
    } else if (p == LNULL) {

        last(*L)->next = q;

    } else if (p == *L) {

        q->next = p;
        *L = q;

    } else {

        q->next = p->next;
        p->next = q;
        q->data = p->data;
        p->data = d;
    }
    return true;
}

bool createNode(tPosL *p) {

    *p = malloc(sizeof(struct Node));

    return *p != LNULL;
}

tPosL deleteAtPosition(tPosL p, tList *L) {

    free(getItem(p, *L));
    if (p == *L) {

        *L = p->next;
    } else if (p->next == LNULL) {

        previous(p, *L)->next = LNULL;
    } else {
        tPosL q;
        q = p->next;
        p->data = q->data;

        p->next = q->next;
        p = q;
    }
    tPosL k = p->next;
    free(p);
    return k;
}

void deleteList(tList *L) {

    tPosL p;
    while (*L != LNULL) {
        p = *L;
        *L = (*L)->next;
        free(getItem(p, *L));
        free(p);

    }
}

int sizeList(tList *L) {

    int size = 0;
    tPosL p;
    if (!isEmptyList(*L)) {
        p = *L;
        while (p != LNULL) {

            p = p->next;
            size++;


        }

    }
    return size;

}