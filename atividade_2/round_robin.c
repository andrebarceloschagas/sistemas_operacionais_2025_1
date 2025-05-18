/* Algoritmo de Escalonamento Round Robin (RR)
 * Este programa simula o escalonamento de processos pelo critério Round Robin, onde cada processo recebe um quantum de tempo para execução.
 * O algoritmo gerencia a fila de processos prontos, alternando entre eles até que todos sejam finalizados.
 * Autor: Antonio André Barcelos Chagas
 * Data: 17/09/2023
 *
 * Variáveis principais:
 * - p[MAX]: vetor de processos
 * - n: quantidade de processos  
 * - quantum: tempo máximo de execução por vez para cada processo
 * - tempo_atual: controla o tempo global de execução
 * - processos_finalizados: conta quantos processos já foram concluídos
 * - fila[MAX]: fila circular de índices dos processos prontos
 * - inicio_fila, fim_fila: controlam o início e fim da fila
 * - em_fila[MAX]: indica se o processo já está na fila
 * - soma_espera, soma_retorno: acumuladores para médias
 */

#include <stdio.h>

#define MAX 100

// Estrutura que representa um processo
typedef struct {
    int id;           // Identificador do processo
    int chegada;      // Tempo de chegada do processo
    int duracao;      // Duração (tempo de execução) do processo
    int restante;     // Tempo restante de execução
    int inicio;       // Momento em que o processo começa a executar
    int termino;      // Momento em que o processo termina
    int espera;       // Tempo de espera na fila
    int retorno;      // Tempo de retorno (turnaround)
    int finalizado;   // Indica se o processo já foi finalizado (1) ou não (0)
} Processo;

int main() {
    Processo p[MAX];
    int n, i, quantum;
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
        p[i].restante = p[i].duracao; // Inicializa o tempo restante
        p[i].inicio = -1;             // Ainda não iniciou
        p[i].finalizado = 0;          // Ainda não finalizado
    }

    printf("Informe o quantum: ");
    scanf("%d", &quantum);

    int fila[MAX];
    int inicio_fila = 0, fim_fila = 0;
    int em_fila[MAX] = {0}; // Marca se o processo já está na fila

    // Adiciona processos que chegam no tempo 0 à fila
    for (i = 0; i < n; i++) {
        if (p[i].chegada == 0) {
            fila[fim_fila++] = i;
            em_fila[i] = 1;
        }
    }

    // Loop principal: executa enquanto houver processos não finalizados
    while (processos_finalizados < n) {
        if (inicio_fila == fim_fila) {
            // Nenhum processo pronto, avança o tempo para o próximo a chegar
            int menor_chegada = -1;
            for (i = 0; i < n; i++) {
                if (!p[i].finalizado && !em_fila[i]) {
                    if (menor_chegada == -1 || p[i].chegada < menor_chegada)
                        menor_chegada = p[i].chegada;
                }
            }
            tempo_atual = menor_chegada;
            for (i = 0; i < n; i++) {
                if (!p[i].finalizado && !em_fila[i] && p[i].chegada == tempo_atual) {
                    fila[fim_fila++] = i;
                    em_fila[i] = 1;
                }
            }
            continue;
        }
        int idx = fila[inicio_fila++]; // Retira o próximo processo da fila
        if (p[idx].inicio == -1)
            p[idx].inicio = tempo_atual; // Marca o início da execução
        int exec = (p[idx].restante < quantum) ? p[idx].restante : quantum;
        tempo_atual += exec;
        p[idx].restante -= exec;
        // Adiciona novos processos que chegaram durante a execução
        for (i = 0; i < n; i++) {
            if (!p[i].finalizado && !em_fila[i] && p[i].chegada <= tempo_atual) {
                fila[fim_fila++] = i;
                em_fila[i] = 1;
            }
        }
        if (p[idx].restante == 0) {
            p[idx].termino = tempo_atual;
            p[idx].espera = p[idx].termino - p[idx].chegada - p[idx].duracao;
            p[idx].retorno = p[idx].termino - p[idx].chegada;
            p[idx].finalizado = 1;
            processos_finalizados++;
        } else {
            fila[fim_fila++] = idx; // Reinsere o processo no final da fila
        }
    }

    // Exibe os resultados
    printf("\nID\tChegada\tDuração\tInício\tTérmino\tEspera\tRetorno\n");
    for (i = 0; i < n; i++) {
        printf("%d\t%d\t\t%d\t\t%d\t%d\t%d\t%d\n", p[i].id, p[i].chegada, p[i].duracao, p[i].inicio, p[i].termino, p[i].espera, p[i].retorno);
        soma_espera += p[i].espera;
        soma_retorno += p[i].retorno;
    }
    printf("\nTempo médio de espera: %.2f\n", soma_espera / n);
    printf("Tempo médio de retorno: %.2f\n", soma_retorno / n);

    return 0;
}
