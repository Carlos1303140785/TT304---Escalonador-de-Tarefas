/* fila de prioridade limitada feita como lista ordenada */
#ifndef FILA_H
#define FILA_H

#include "task.h"

typedef struct FilaPrioridade {
    Task *inicio;      /* tarefa de maior prioridade */
    int   tamanho;     /* quantidade atual de tarefas */
    int   capacidade;  /* limite de tarefas da fila */
} FilaPrioridade;

/* cria uma fila vazia ou devolve NULL em caso de erro */
FilaPrioridade *fila_criar(int capacidade);

/* destroi a fila e as tarefas que ainda estiverem nela */
void fila_destruir(FilaPrioridade *fila);

#endif
