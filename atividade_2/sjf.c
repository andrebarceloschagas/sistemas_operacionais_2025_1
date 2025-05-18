/*
 * Algoritmo de Escalonamento SJF (Shortest Job First)
 * Este programa simula o escalonamento de processos pelo critério do menor trabalho primeiro (menor duração).
 * Cada processo possui tempo de chegada, duração, e o programa calcula tempos de início, término, espera e retorno.
 * Autor: Antonio André Barcelos Chagas
 * Data: 17/09/2023
 *
 * Variáveis principais:
 * - p[MAX]: vetor de processos
 * - n: quantidade de processos
 * - tempo_atual: controla o tempo global de execução
 * - processos_finalizados: conta quantos processos já foram concluídos
 * - soma_espera, soma_retorno: acumuladores para médias
 */

#include <stdio.h>

#define MAX 100

// Estrutura que representa um processo
typedef struct {
    int id;           // Identificador do processo
    int chegada;      // Tempo de chegada do processo
    int duracao;      // Duração (tempo de execução) do processo
    int inicio;       // Momento em que o processo começa a executar
    int termino;      // Momento em que o processo termina
    int espera;       // Tempo de espera na fila
    int retorno;      // Tempo de retorno (turnaround)
    int finalizado;   // Indica se o processo já foi finalizado (1) ou não (0)
} Processo;

int main() {
    Processo p[MAX];
    int n, i;
    int tempo_atual = 0;
    int processos_finalizados = 0;
    float soma_espera = 0, soma_retorno = 0;

    printf("Informe o número de processos: ");
    scanf("%d", &n);

    // Entrada dos dados dos processos
    for (i = 0; i < n; i++) {
        p[i].id = i + 1;
        printf("Processo %d - Tempo de chegada: ", p[i].id);
        scanf("%d", &p[i].chegada);
        printf("Processo %d - Duração: ", p[i].id);
        scanf("%d", &p[i].duracao);
        p[i].finalizado = 0; // Inicialmente, nenhum processo está finalizado
    }

    // Loop principal: executa até todos os processos serem finalizados
    while (processos_finalizados < n) {
        int idx = -1;
        int menor_duracao = -1;
        // Busca o processo disponível com menor duração
        for (i = 0; i < n; i++) {
            if (!p[i].finalizado && p[i].chegada <= tempo_atual) {
                if (menor_duracao == -1 || p[i].duracao < menor_duracao) {
                    menor_duracao = p[i].duracao;
                    idx = i;
                }
            }
        }
        // Se nenhum processo está pronto, avança o tempo para o próximo processo que chegar
        if (idx == -1) {
            int menor_chegada = -1;
            for (i = 0; i < n; i++) {
                if (!p[i].finalizado) {
                    if (menor_chegada == -1 || p[i].chegada < menor_chegada)
                        menor_chegada = p[i].chegada;
                }
            }
            tempo_atual = menor_chegada;
            continue;
        }
        // Executa o processo selecionado (menor duração entre os disponíveis)
        p[idx].inicio = tempo_atual;
        p[idx].termino = p[idx].inicio + p[idx].duracao;
        p[idx].espera = p[idx].inicio - p[idx].chegada;
        p[idx].retorno = p[idx].termino - p[idx].chegada;
        tempo_atual = p[idx].termino;
        p[idx].finalizado = 1;
        processos_finalizados++;
        soma_espera += p[idx].espera;
        soma_retorno += p[idx].retorno;
    }

    // Exibe os resultados
    printf("\nID\tChegada\tDuração\tInício\tTérmino\tEspera\tRetorno\n");
    for (i = 0; i < n; i++) {
        printf("%d\t%d\t\t%d\t\t%d\t%d\t%d\t%d\n", p[i].id, p[i].chegada, p[i].duracao, p[i].inicio, p[i].termino, p[i].espera, p[i].retorno);
    }
    printf("\nTempo médio de espera: %.2f\n", soma_espera / n);
    printf("Tempo médio de retorno: %.2f\n", soma_retorno / n);

    return 0;
}