/* implementacao da fila de prioridade com sincronizacao */
#include <stdlib.h>
#include "fila.h"

/* as duas funcoes a seguir exigem o mutex ja travado */

static void inserir_ordenado(FilaPrioridade *fila, Task *tarefa)
{
    /* pp aponta para o ponteiro que sera reescrito */
    /* o menor ou igual mantem a ordem de chegada nos empates */
    Task **pp = &fila->inicio;
    while (*pp != NULL && (*pp)->tempo_execucao <= tarefa->tempo_execucao)
        pp = &(*pp)->proximo;

    tarefa->proximo = *pp;
    *pp = tarefa;
    fila->tamanho++;
}

static Task *remover_primeiro(FilaPrioridade *fila)
{
    Task *t = fila->inicio;
    fila->inicio = t->proximo;
    t->proximo = NULL;
    fila->tamanho--;
    return t;
}

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

    /* desfaz o que ja foi criado se algum init falhar */
    if (pthread_mutex_init(&f->mutex, NULL) != 0) {
        free(f);
        return NULL;
    }
    if (pthread_cond_init(&f->nao_cheia, NULL) != 0) {
        pthread_mutex_destroy(&f->mutex);
        free(f);
        return NULL;
    }
    if (pthread_cond_init(&f->nao_vazia, NULL) != 0) {
        pthread_cond_destroy(&f->nao_cheia);
        pthread_mutex_destroy(&f->mutex);
        free(f);
        return NULL;
    }
    return f;
}

void fila_destruir(FilaPrioridade *fila)
{
    if (fila == NULL)
        return;

    Task *atual = fila->inicio;
    while (atual != NULL) {
        Task *proxima = atual->proximo;
        task_destruir(atual);
        atual = proxima;
    }
    pthread_cond_destroy(&fila->nao_vazia);
    pthread_cond_destroy(&fila->nao_cheia);
    pthread_mutex_destroy(&fila->mutex);
    free(fila);
}

int fila_inserir(FilaPrioridade *fila, Task *tarefa)
{
    if (fila == NULL || tarefa == NULL)
        return -1;

    pthread_mutex_lock(&fila->mutex);

    /* o while trata despertares espurios */
    while (fila->tamanho >= fila->capacidade)
        pthread_cond_wait(&fila->nao_cheia, &fila->mutex);

    inserir_ordenado(fila, tarefa);

    /* entrou uma tarefa so um consumidor precisa acordar */
    pthread_cond_signal(&fila->nao_vazia);
    pthread_mutex_unlock(&fila->mutex);
    return 0;
}

Task *fila_remover(FilaPrioridade *fila)
{
    if (fila == NULL)
        return NULL;

    pthread_mutex_lock(&fila->mutex);

    while (fila->tamanho == 0)
        pthread_cond_wait(&fila->nao_vazia, &fila->mutex);

    Task *t = remover_primeiro(fila);

    /* abriu uma vaga so o produtor precisa acordar */
    pthread_cond_signal(&fila->nao_cheia);
    pthread_mutex_unlock(&fila->mutex);
    return t;
}
