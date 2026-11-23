/* escalonador de tarefas com pool de threads e fila de prioridade */
/* uso e escalonador numero de consumidores e arquivo de entrada */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>

#include "fila.h"
#include "produtor.h"
#include "consumidor.h"

/* capacidade da fila ajustavel na compilacao */
#ifndef MAX_TASKS
#define MAX_TASKS 20
#endif

/* converte e valida o numero de consumidores e devolve zero se invalido */
static int ler_num_consumidores(const char *texto)
{
    char *fim = NULL;
    errno = 0;
    long n = strtol(texto, &fim, 10);

    if (errno != 0 || fim == texto || *fim != '\0' || n < 1 || n > 10000)
        return 0;
    return (int)n;
}

/* encerra os k consumidores ja criados quando algo falha */
static void encerrar_consumidores(FilaPrioridade *fila, pthread_t *threads, int k)
{
    for (int i = 0; i < k; i++)
        fila_inserir(fila, task_criar_fim());
    for (int i = 0; i < k; i++)
        pthread_join(threads[i], NULL);
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <num_consumidores> <arquivo_de_entrada>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int n = ler_num_consumidores(argv[1]);
    if (n == 0) {
        fprintf(stderr, "Erro: número de consumidores inválido (use 1..10000).\n");
        return EXIT_FAILURE;
    }

    /* cada estrutura e alocada em uma unica etapa */
    FilaPrioridade *fila       = fila_criar(MAX_TASKS);
    pthread_t      *threads    = malloc((size_t)n * sizeof *threads);
    pthread_t      *th_prod    = malloc(sizeof *th_prod);
    ArgsConsumidor *args_cons  = malloc((size_t)n * sizeof *args_cons);
    ArgsProdutor   *args_prod  = malloc(sizeof *args_prod);

    int status = EXIT_FAILURE;

    if (fila == NULL || threads == NULL || th_prod == NULL ||
        args_cons == NULL || args_prod == NULL) {
        fprintf(stderr, "Erro: memória insuficiente.\n");
        goto limpeza;
    }

    args_prod->fila             = fila;
    args_prod->arquivo          = argv[2];
    args_prod->num_consumidores = n;
    args_prod->status           = 0;

    /* primeiro os consumidores */
    for (int i = 0; i < n; i++) {
        args_cons[i].fila = fila;
        args_cons[i].tid  = i + 1;
        if (pthread_create(&threads[i], NULL, consumidor_thread, &args_cons[i]) != 0) {
            fprintf(stderr, "Erro: falha ao criar o consumidor %d.\n", i + 1);
            encerrar_consumidores(fila, threads, i);
            goto limpeza;
        }
    }

    /* depois o produtor */
    if (pthread_create(th_prod, NULL, produtor_thread, args_prod) != 0) {
        fprintf(stderr, "Erro: falha ao criar o produtor.\n");
        encerrar_consumidores(fila, threads, n);
        goto limpeza;
    }

    /* espera todos os threads terminarem */
    pthread_join(*th_prod, NULL);
    for (int i = 0; i < n; i++)
        pthread_join(threads[i], NULL);

    printf("Todas as tarefas foram processadas. Encerrando.\n");
    status = (args_prod->status == 0) ? EXIT_SUCCESS : EXIT_FAILURE;

limpeza:
    /* libera tudo que foi alocado */
    fila_destruir(fila);
    free(args_prod);
    free(args_cons);
    free(th_prod);
    free(threads);
    return status;
}
