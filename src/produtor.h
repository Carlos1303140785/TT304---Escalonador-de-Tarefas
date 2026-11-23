/* thread produtor que le o arquivo e alimenta a fila */
#ifndef PRODUTOR_H
#define PRODUTOR_H

#include "fila.h"

typedef struct ArgsProdutor {
    FilaPrioridade *fila;              /* fila compartilhada */
    const char     *arquivo;           /* caminho do arquivo de entrada */
    int             num_consumidores;  /* quantidade de sentinelas a inserir */
    int             status;            /* zero se tudo certo */
} ArgsProdutor;

/* le o arquivo enfileira as tarefas e por fim insere as sentinelas */
void *produtor_thread(void *arg);

#endif
