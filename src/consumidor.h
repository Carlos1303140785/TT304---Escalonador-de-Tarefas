/* threads consumidores do pool de trabalhadores */
#ifndef CONSUMIDOR_H
#define CONSUMIDOR_H

#include "fila.h"

typedef struct ArgsConsumidor {
    FilaPrioridade *fila;  /* fila compartilhada */
    int             tid;   /* numero do consumidor de um ate N */
} ArgsConsumidor;

/* retira tarefas executa e libera ate achar uma sentinela */
void *consumidor_thread(void *arg);

#endif
