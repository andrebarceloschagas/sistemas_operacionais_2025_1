/* Algoritmo de Escalonamento FCFS (First-Come, First-Served)
 * Este programa simula o escalonamento de processos pelo critério de chegada (primeiro a chegar, primeiro a ser atendido).
 * Cada processo possui tempo de chegada, duração, e o programa calcula tempos de início, término, espera e retorno.
 * Autor: Antonio André Barcelos Chagas
 * Data: 17/09/2023
 *
 * Variáveis principais:
 * - p[MAX]: vetor de processos
 * - n: quantidade de processos  
 * - tempo_atual: controla o tempo global de execução
 * - soma_espera, soma_retorno: acumuladores para médias
 */

#include <stdio.h>

#define MAX 100

// Estrutura que representa um processo
typedef struct {
    int id;         // Identificador do processo
    int chegada;    // Tempo de chegada do processo
    int duracao;    // Duração (tempo de execução) do processo
    int inicio;     // Momento em que o processo começa a executar
    int termino;    // Momento em que o processo termina
    int espera;     // Tempo de espera na fila
    int retorno;    // Tempo de retorno (turnaround)
} Processo;

int main() {
    Processo p[MAX];
    int n, i;
    int tempo_atual = 0;
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
    }

    // Ordena os processos pelo tempo de chegada (FCFS)
    for (i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (p[j].chegada < p[i].chegada) {
                Processo aux = p[i];
                p[i] = p[j];
                p[j] = aux;
            }
        }
    }

    // Calcula os tempos de início, término, espera e retorno para cada processo
    for (i = 0; i < n; i++) {
        if (tempo_atual < p[i].chegada)
            tempo_atual = p[i].chegada; // Se o processo chegou depois do tempo atual, avança o tempo
        p[i].inicio = tempo_atual;
        p[i].termino = p[i].inicio + p[i].duracao;
        p[i].espera = p[i].inicio - p[i].chegada;
        p[i].retorno = p[i].termino - p[i].chegada;
        tempo_atual = p[i].termino;
        soma_espera += p[i].espera;
        soma_retorno += p[i].retorno;
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
