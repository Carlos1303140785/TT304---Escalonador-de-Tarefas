/* implementacao do thread consumidor */
#include <stdio.h>
#include <unistd.h>
#include "consumidor.h"

void *consumidor_thread(void *arg)
{
    ArgsConsumidor *a = arg;

    for (;;) {
        /* bloqueia enquanto a fila estiver vazia */
        Task *t = fila_remover(a->fila);

        if (t == NULL || task_eh_fim(t)) {
            task_destruir(t);
            break;
        }

        printf("Consumidor %d: iniciando tarefa %d (duração %d s).\n",
               a->tid, t->id, t->tempo_execucao);
        fflush(stdout);

        /* simula a execucao da tarefa */
        sleep((unsigned int)t->tempo_execucao);

        printf("Consumidor %d: tarefa %d concluída.\n", a->tid, t->id);
        fflush(stdout);

        /* libera o buffer e a estrutura da tarefa */
        task_destruir(t);
    }
    return NULL;
}
