/* fila de prioridade limitada e segura para varios threads */
#ifndef FILA_H
#define FILA_H

#include <pthread.h>
#include "task.h"

typedef struct FilaPrioridade {
    Task           *inicio;      /* tarefa de maior prioridade */
    int             tamanho;     /* quantidade atual de tarefas */
    int             capacidade;  /* limite de tarefas da fila */
    pthread_mutex_t mutex;       /* protege todos os campos da fila */
    pthread_cond_t  nao_cheia;   /* avisa que abriu uma vaga */
    pthread_cond_t  nao_vazia;   /* avisa que entrou uma tarefa */
} FilaPrioridade;

/* cria uma fila vazia ou devolve NULL em caso de erro */
FilaPrioridade *fila_criar(int capacidade);

/* destroi a fila e as tarefas restantes sem nenhum thread usando */
void fila_destruir(FilaPrioridade *fila);

/* insere a tarefa e bloqueia enquanto a fila estiver cheia */
int fila_inserir(FilaPrioridade *fila, Task *tarefa);

/* remove a tarefa de maior prioridade e bloqueia enquanto vazia */
Task *fila_remover(FilaPrioridade *fila);

#endif
