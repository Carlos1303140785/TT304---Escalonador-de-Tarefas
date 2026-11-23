/* implementacao do thread produtor */
#include <stdio.h>
#include <stdlib.h>
#include "produtor.h"

/* Insere de forma estavel: tarefas de mesma duracao mantem FIFO. */
static void inserir_ordenado(Task **inicio, Task *tarefa)
{
    Task **posicao = inicio;

    while (*posicao != NULL &&
           (*posicao)->tempo_execucao <= tarefa->tempo_execucao) {
        posicao = &(*posicao)->proximo;
    }
    tarefa->proximo = *posicao;
    *posicao = tarefa;
}

static void liberar_lista(Task *inicio)
{
    while (inicio != NULL) {
        Task *proxima = inicio->proximo;
        task_destruir(inicio);
        inicio = proxima;
    }
}

/* Insere uma sentinela por consumidor e devolve o numero de falhas. */
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
    Task *tarefas = NULL;
    a->status = 0;

    FILE *f = fopen(a->arquivo, "r");
    if (f == NULL) {
        perror("Produtor: nao foi possivel abrir o arquivo de entrada");
        a->status = 1;
    } else {
        int id, tempo;
        int linha = 0;
        int lidos;

        /* Lê todas as tarefas antes de liberar qualquer uma aos consumidores.
         * Isso permite que a primeira retirada considere prioridades do arquivo
         * inteiro, sem aumentar a capacidade da fila compartilhada.
         */
        while ((lidos = fscanf(f, "%d %d", &id, &tempo)) == 2) {
            linha++;
            Task *t = task_criar(id, tempo);
            if (t == NULL) {
                fprintf(stderr, "Produtor: entrada %d invalida ou sem memoria.\n", linha);
                a->status = 1;
                continue;
            }
            inserir_ordenado(&tarefas, t);
        }
        if (lidos != EOF) {
            fprintf(stderr, "Produtor: formato invalido apos a entrada %d.\n", linha);
            a->status = 1;
        }
        fclose(f);
    }

    /* Transfere a lista ordenada para a bounded buffer. Se houver mais tarefas
     * que vagas, fila_inserir bloqueia ate um consumidor remover uma tarefa.
     */
    while (tarefas != NULL) {
        Task *t = tarefas;
        tarefas = t->proximo;
        t->proximo = NULL;

        if (fila_inserir(a->fila, t) != 0) {
            fprintf(stderr, "Produtor: erro ao enfileirar tarefa %d.\n", t->id);
            task_destruir(t);
            a->status = 1;
            liberar_lista(tarefas);
            tarefas = NULL;
            break;
        }
        printf("Produtor: tarefa %d alocada e enfileirada.\n", t->id);
        fflush(stdout);
    }

    /* As sentinelas possuem duracao INT_MAX e entram depois das tarefas reais. */
    if (enfileirar_sentinelas(a) != 0)
        a->status = 1;
    return NULL;
}
