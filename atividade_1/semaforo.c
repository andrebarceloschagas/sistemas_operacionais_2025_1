#define __USE_GNU 1
// Define uma macro para habilitar funcionalidades específicas do GNU.

#include <pthread.h>  // Biblioteca para manipulação de threads.
#include <stdio.h>    // Biblioteca padrão para entrada e saída.
#include <stdlib.h>   // Biblioteca padrão para funções utilitárias.
#include <unistd.h>   // Biblioteca para funções de tempo e chamadas ao sistema.
#include <semaphore.h> // Biblioteca para manipulação de semáforos.

#define N_ITENS 30
// Define o tamanho do buffer circular como 30.

int buffer[N_ITENS];
// Declara o buffer circular que será compartilhado entre o produtor e o consumidor.

sem_t pos_vazia;
// Semáforo que controla as posições vazias no buffer.

sem_t pos_ocupada;
// Semáforo que controla as posições ocupadas no buffer.

int inicio = 0, final = 0;
// `inicio` e `final` controlam as posições no buffer circular.

void* produtor(void *v) {
    // Função que será executada pela thread do produtor.
    int i;
    for (i = 0; i < 3 * N_ITENS; i++) {
        // Produz 3 vezes o número de itens do buffer.

        sem_wait(&pos_vazia);
        // Decrementa o semáforo `pos_vazia`. Aguarda até que haja espaço no buffer.

        printf("Produtor, item = %d.\n", i);
        // Imprime o item que está sendo produzido.

        final = (final + 1) % N_ITENS;
        // Atualiza a posição final no buffer circular.

        buffer[final] = i;
        // Armazena o item produzido no buffer.

        sem_post(&pos_ocupada);
        // Incrementa o semáforo `pos_ocupada`, indicando que há um novo item no buffer.

        sleep(random() % 3);
        // Faz uma pausa aleatória (0 a 2 segundos) para simular o tempo de produção.
    }
    return NULL;
    // Retorna NULL ao final da execução.
}

void* consumidor(void *v) {
    // Função que será executada pela thread do consumidor.
    int i;
    for (i = 0; i < 3 * N_ITENS; i++) {
        // Consome 3 vezes o número de itens do buffer.

        sem_wait(&pos_ocupada);
        // Decrementa o semáforo `pos_ocupada`. Aguarda até que haja itens no buffer.

        inicio = (inicio + 1) % N_ITENS;
        // Atualiza a posição inicial no buffer circular.

        printf("Consumidor, item = %d.\n", buffer[inicio]);
        // Imprime o item que está sendo consumido.

        sem_post(&pos_vazia);
        // Incrementa o semáforo `pos_vazia`, indicando que há uma nova posição vazia no buffer.

        sleep(random() % 3);
        // Faz uma pausa aleatória (0 a 2 segundos) para simular o tempo de consumo.
    }
    return NULL;
    // Retorna NULL ao final da execução.
}

int main() {
    // Função principal do programa.
    pthread_t thr_produtor, thr_consumidor;
    // Declara duas variáveis para armazenar os identificadores das threads.

    sem_init(&pos_vazia, 0, N_ITENS);
    // Inicializa o semáforo `pos_vazia` com o valor `N_ITENS` (todas as posições estão inicialmente vazias).

    sem_init(&pos_ocupada, 0, 0);
    // Inicializa o semáforo `pos_ocupada` com o valor 0 (nenhuma posição está ocupada inicialmente).

    pthread_create(&thr_produtor, NULL, produtor, NULL);
    // Cria a thread do produtor e a associa à função `produtor`.

    pthread_create(&thr_consumidor, NULL, consumidor, NULL);
    // Cria a thread do consumidor e a associa à função `consumidor`.

    pthread_join(thr_produtor, NULL);
    // Aguarda a conclusão da thread do produtor.

    pthread_join(thr_consumidor, NULL);
    // Aguarda a conclusão da thread do consumidor.

    sem_destroy(&pos_vazia);
    // Destroi o semáforo `pos_vazia`.

    sem_destroy(&pos_ocupada);
    // Destroi o semáforo `pos_ocupada`.

    return 0;
    // Retorna 0 indicando que o programa foi executado com sucesso.
}