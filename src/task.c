/* implementacao do modulo da tarefa */
#include <stdlib.h>
#include "task.h"

Task *task_criar(int id, int tempo_execucao)
{
    /* ids negativos sao reservados e tempo negativo e invalido */
    if (id < 0 || tempo_execucao < 0)
        return NULL;

    Task *t = malloc(sizeof *t);
    if (t == NULL)
        return NULL;

    t->id             = id;
    t->tempo_execucao = tempo_execucao;
    t->proximo        = NULL;
    t->tamanho_dados  = (size_t)tempo_execucao * TASK_BYTES_POR_SEGUNDO;

    /* calloc ja zera o buffer e com tempo zero reserva um byte */
    t->dados = calloc(t->tamanho_dados > 0 ? t->tamanho_dados : 1, 1);
    if (t->dados == NULL) {
        free(t);
        return NULL;
    }
    return t;
}

Task *task_criar_fim(void)
{
    Task *t = malloc(sizeof *t);
    if (t == NULL)
        return NULL;

    t->id             = TASK_ID_FIM;
    t->tempo_execucao = TASK_TEMPO_FIM;
    t->dados          = NULL;
    t->tamanho_dados  = 0;
    t->proximo        = NULL;
    return t;
}

int task_eh_fim(const Task *t)
{
    return t != NULL && t->id < 0;
}

void task_destruir(Task *t)
{
    if (t == NULL)
        return;
    free(t->dados);
    free(t);
}
