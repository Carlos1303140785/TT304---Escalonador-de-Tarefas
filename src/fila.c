/* implementacao da fila de prioridade */
#include <stdlib.h>
#include "fila.h"

FilaPrioridade *fila_criar(int capacidade)
{
    if (capacidade <= 0)
        return NULL;

    FilaPrioridade *f = malloc(sizeof *f);
    if (f == NULL)
        return NULL;

    f->inicio     = NULL;
    f->tamanho    = 0;
    f->capacidade = capacidade;
    return f;
}

void fila_destruir(FilaPrioridade *fila)
{
    if (fila == NULL)
        return;

    /* libera cada tarefa que sobrou antes da fila */
    Task *atual = fila->inicio;
    while (atual != NULL) {
        Task *proxima = atual->proximo;
        task_destruir(atual);
        atual = proxima;
    }
    free(fila);
}
