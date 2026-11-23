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

int fila_inserir_ordenado(FilaPrioridade *fila, Task *tarefa)
{
    if (fila == NULL || tarefa == NULL || fila->tamanho >= fila->capacidade)
        return -1;

    /* pp aponta para o ponteiro que sera reescrito */
    /* o menor ou igual mantem a ordem de chegada nos empates */
    Task **pp = &fila->inicio;
    while (*pp != NULL && (*pp)->tempo_execucao <= tarefa->tempo_execucao)
        pp = &(*pp)->proximo;

    tarefa->proximo = *pp;
    *pp = tarefa;
    fila->tamanho++;
    return 0;
}

Task *fila_remover_primeiro(FilaPrioridade *fila)
{
    if (fila == NULL || fila->inicio == NULL)
        return NULL;

    Task *t = fila->inicio;
    fila->inicio = t->proximo;
    t->proximo = NULL;
    fila->tamanho--;
    return t;
}
