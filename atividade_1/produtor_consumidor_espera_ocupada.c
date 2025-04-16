/*
 * Código para produtor e consumidor com uma tentativa de
 * controle de concorrência baseada em espera ocupada.
 */
// Este comentário inicial explica que o código tenta implementar controle de concorrência
// usando espera ocupada, o que não é eficiente.

#include <pthread.h>  // Biblioteca para manipulação de threads.
#include <stdio.h>    // Biblioteca padrão para entrada e saída.
#include <stdlib.h>   // Biblioteca padrão para funções utilitárias.
#include <unistd.h>   // Biblioteca para funções de tempo e chamadas ao sistema.

#define N_ITENS 10
// Define o tamanho do buffer circular como 10.

int buffer[N_ITENS];
// Declara o buffer circular que será compartilhado entre o produtor e o consumidor.

int inicio = 0, final = 0, cont = 0;
// `inicio` e `final` controlam as posições no buffer circular.
// `cont` mantém o número de itens atualmente no buffer.

void* produtor(void *v) {
    // Função que será executada pela thread do produtor.
    int i, aux;
    for (i = 0; i < 3 * N_ITENS; i++) {
        // Produz 3 vezes o número de itens do buffer.
        while (cont == N_ITENS);
        // Espera ocupada: aguarda até que haja espaço no buffer.

        printf("Produtor, item = %d.\n", i);
        // Imprime o item que está sendo produzido.

        final = (final + 1) % N_ITENS;
        // Atualiza a posição final no buffer circular.

        buffer[final] = i;
        // Armazena o item produzido no buffer.

        aux = cont;
        // Salva o valor atual de `cont` em uma variável auxiliar.

        sleep(random() % 3);
        // Faz uma pausa aleatória (0 a 2 segundos) para simular o tempo de produção.

        cont = aux + 1;
        // Incrementa o contador de itens no buffer.
    }
    printf("Produção encerrada.\n");
    // Indica que a produção foi concluída.

    return NULL;
    // Retorna NULL ao final da execução.
}

void* consumidor(void *v) {
    // Função que será executada pela thread do consumidor.
    int i, aux;
    for (i = 0; i < 3 * N_ITENS; i++) {
        // Consome 3 vezes o número de itens do buffer.
        while (cont == 0);
        // Espera ocupada: aguarda até que haja itens no buffer.

        inicio = (inicio + 1) % N_ITENS;
        // Atualiza a posição inicial no buffer circular.

        aux = buffer[inicio];
        // Remove o item do buffer e o armazena em `aux`.

        printf("Consumidor, item = %d.\n", aux);
        // Imprime o item que está sendo consumido.

        if (aux < i)
            printf("Erro: item foi consumido antes de ser produzido.\n");
        // Verifica se o item foi consumido antes de ser produzido.

        else if (aux > i)
            printf("Erro: item foi produzido antes de a posição estar livre.\n");
        // Verifica se o item foi produzido antes de a posição estar livre.

        aux = cont;
        // Salva o valor atual de `cont` em uma variável auxiliar.

        sleep(random() % 3);
        // Faz uma pausa aleatória (0 a 2 segundos) para simular o tempo de consumo.

        cont = aux - 1;
        // Decrementa o contador de itens no buffer.
    }
    printf("Consumo encerrado.\n");
    // Indica que o consumo foi concluído.

    return NULL;
    // Retorna NULL ao final da execução.
}

int main() {
    // Função principal do programa.
    int i;
    pthread_t thr_produtor, thr_consumidor;
    // Declara duas variáveis para armazenar os identificadores das threads.

    /* Limpa o buffer */
    for (i = 0; i < N_ITENS; i++)
        buffer[i] = 0;
        // Inicializa o buffer com zeros.

    pthread_create(&thr_produtor, NULL, produtor, NULL);
    // Cria a thread do produtor e a associa à função `produtor`.

    pthread_create(&thr_consumidor, NULL, consumidor, NULL);
    // Cria a thread do consumidor e a associa à função `consumidor`.

    pthread_join(thr_produtor, NULL);
    // Aguarda a conclusão da thread do produtor.

    pthread_join(thr_consumidor, NULL);
    // Aguarda a conclusão da thread do consumidor.

    return 0;
    // Retorna 0 indicando que o programa foi executado com sucesso.
}