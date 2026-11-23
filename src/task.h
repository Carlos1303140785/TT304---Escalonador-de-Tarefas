/* modulo da tarefa com buffer de dados dinamico */
#ifndef TASK_H
#define TASK_H

#include <stddef.h>
#include <limits.h>

/* identificador reservado das tarefas sentinela */
#define TASK_ID_FIM (-1)

/* tempo maximo para a sentinela ficar sempre depois das tarefas reais */
#define TASK_TEMPO_FIM INT_MAX

/* bytes de buffer por segundo de execucao */
#define TASK_BYTES_POR_SEGUNDO 10

typedef struct Task {
    int            id;              /* identificador da tarefa */
    int            tempo_execucao;  /* tempo simulado em segundos */
    unsigned char *dados;           /* buffer de dados zerado */
    size_t         tamanho_dados;   /* tamanho do buffer em bytes */
    struct Task   *proximo;         /* proxima tarefa na fila */
} Task;

/* cria uma tarefa real ou devolve NULL em caso de erro */
Task *task_criar(int id, int tempo_execucao);

/* cria uma tarefa sentinela sem buffer */
Task *task_criar_fim(void);

/* diz se a tarefa sinaliza o fim do trabalho */
int task_eh_fim(const Task *t);

/* libera o buffer e a estrutura da tarefa */
void task_destruir(Task *t);

#endif
