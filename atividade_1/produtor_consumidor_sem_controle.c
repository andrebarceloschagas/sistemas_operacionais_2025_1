/*
 * Código para produtor e consumidor sem nenhum tipo de controle de concorrência.
 */

#include <pthread.h>  // Biblioteca para manipulação de threads.
#include <stdio.h>    // Biblioteca padrão para entrada e saída.
#include <stdlib.h>   // Biblioteca padrão para funções utilitárias.
#include <unistd.h>   // Biblioteca para funções de tempo e chamadas ao sistema.

#define N_ITENS 10
// Define o tamanho do buffer circular como 10.
int buffer[N_ITENS];
// Declara o buffer circular que será compartilhado entre o produtor e o consumidor.

int inicio = 0, final = 0;
// Variáveis que controlam o início e o final do buffer circular.

void* produtor(void *v) {
    // Função que será executada pela thread do produtor.
    int i;
    for (i = 0; i < 3 * N_ITENS; i++) {
        // Produz 3 vezes o número de itens do buffer.
        printf("Produtor, item = %d.\n", i);
        // Imprime o item que está sendo produzido.
        final = (final + 1) % N_ITENS;
        // Atualiza a posição final no buffer circular.
        buffer[final] = i;
        // Armazena o item produzido no buffer.
        sleep(5); /* Permite que a outra thread execute */
        // Faz uma pausa de 5 segundos para permitir que outras threads executem.
    }
    return NULL;
    // Retorna NULL ao final da execução.
}

void* consumidor(void *v) {
    // Função que será executada pela thread do consumidor.
    int i;
    for (i = 0; i < 3 * N_ITENS; i++) {
        // Consome 3 vezes o número de itens do buffer.
        inicio = (inicio + 1) % N_ITENS;
        // Atualiza a posição inicial no buffer circular.
        printf("Consumidor, item = %d.\n", buffer[inicio]);
        // Imprime o item que está sendo consumido.
        sleep(1); /* Permite que a outra thread execute */
        // Faz uma pausa de 1 segundo para permitir que outras threads executem.
    }
    return NULL;
    // Retorna NULL ao final da execução.
}

int main() {
    // Função principal do programa.
    int i;
    pthread_t thr_produtor, thr_consumidor;
    // Declara duas variáveis para armazenar os identificadores das threads.

    /* Limpa o buffer */
    for (i = 0; i < N_ITENS; i++) {
        buffer[i] = 0;
        // Inicializa o buffer com zeros.
    }

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
