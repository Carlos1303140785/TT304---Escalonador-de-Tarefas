/* implementacao do thread produtor */
#include <stdio.h>
#include "produtor.h"

/* insere uma sentinela por consumidor e devolve o numero de falhas */
static int enfileirar_sentinelas(ArgsProdutor *a)
{
    int falhas = 0;

    for (int i = 0; i < a->num_consumidores; i++) {
        Task *fim = task_criar_fim();
        if (fim == NULL || fila_inserir(a->fila, fim) != 0) {
            fprintf(stderr, "Produtor: erro ao criar sentinela %d.\n", i + 1);
            task_destruir(fim);
            falhas++;
        }
    }
    return falhas;
}

void *produtor_thread(void *arg)
{
    ArgsProdutor *a = arg;
    a->status = 0;

    FILE *f = fopen(a->arquivo, "r");
    if (f == NULL) {
        perror("Produtor: não foi possível abrir o arquivo de entrada");
        a->status = 1;
    } else {
        int id, tempo;
        int linha = 0;
        int lidos;

        while ((lidos = fscanf(f, "%d %d", &id, &tempo)) == 2) {
            linha++;

            /* linha invalida e ignorada e o programa segue */
            Task *t = task_criar(id, tempo);
            if (t == NULL) {
                fprintf(stderr, "Produtor: entrada %d ignorada "
                                "(valores inválidos ou sem memória).\n", linha);
                a->status = 1;
                continue;
            }
            fila_inserir(a->fila, t);
            printf("Produtor: tarefa %d alocada e enfileirada.\n", id);
            fflush(stdout);
        }
        if (lidos != EOF) {
            fprintf(stderr, "Produtor: formato inválido após a entrada %d.\n", linha);
            a->status = 1;
        }
        fclose(f);
    }

    /* as sentinelas sempre saem para os consumidores nao ficarem presos */
    if (enfileirar_sentinelas(a) != 0)
        a->status = 1;
    return NULL;
}
