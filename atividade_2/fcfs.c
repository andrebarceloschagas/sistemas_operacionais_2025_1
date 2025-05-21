/* Algoritmo de Escalonamento FCFS (First-Come, First-Served)
 * Este programa simula o escalonamento de processos pelo critério de chegada (primeiro a chegar, primeiro a ser atendido).
 * Cada processo possui tempo de chegada, duração, e o programa calcula tempos de início, término, espera e retorno.
 * Autor: Antonio André Barcelos Chagas
 * Data: 17/05/2025
 *
 * Variáveis principais:
 * - p[MAX]: vetor de processos
 * - n: quantidade de processos  
 * - tempo_atual: controla o tempo global de execução
 * - soma_espera, soma_retorno: acumuladores para médias
 */

#include <stdio.h>
#include <stdlib.h> // Para malloc e free

// (Estrutura Processo e função comparar_processos permanecem iguais)
typedef struct {
    int id;
    int chegada;
    int duracao;
    int inicio;
    int termino;
    int espera;
    int retorno;
} Processo;

int comparar_processos(const void *a, const void *b) {
    Processo *p1 = (Processo *)a;
    Processo *p2 = (Processo *)b;
    if (p1->chegada < p2->chegada) return -1;
    if (p1->chegada > p2->chegada) return 1;
    if (p1->id < p2->id) return -1;
    if (p1->id > p2->id) return 1;
    return 0;
}


int main() {
    Processo *p; // Ponteiro para Processo, em vez de um array estático
    int n, i;
    int tempo_atual = 0;
    float soma_espera = 0, soma_retorno = 0;

    printf("Algoritmo de Escalonamento FCFS (First-Come, First-Served)\n");
    printf("---------------------------------------------------------\n");
    printf("Informe o número de processos: ");
    if (scanf("%d", &n) != 1 || n <= 0) { // Adicionada verificação de n > 0 aqui
        fprintf(stderr, "Erro: Número de processos deve ser positivo.\n");
        return 1;
    }

    // Alocação dinâmica do array de processos
    p = (Processo *)malloc(n * sizeof(Processo));
    if (p == NULL) {
        fprintf(stderr, "Erro: Falha na alocação de memória!\n");
        return 1; // Encerra se malloc falhar
    }

    // Entrada dos dados dos processos (o resto do código é muito similar)
    printf("\n--- Entrada dos Dados dos Processos ---\n");
    for (i = 0; i < n; i++) {
        p[i].id = i + 1;
        printf("Processo %d:\n", p[i].id);
        printf("  Tempo de chegada: ");
        if (scanf("%d", &p[i].chegada) != 1 || p[i].chegada < 0) {
            fprintf(stderr, "Erro: Entrada inválida para o tempo de chegada.\n");
            free(p); // Libera memória antes de sair
            return 1;
        }
        printf("  Duração (tempo de CPU): ");
        if (scanf("%d", &p[i].duracao) != 1 || p[i].duracao <= 0) {
            fprintf(stderr, "Erro: Entrada inválida para a duração.\n");
            free(p); // Libera memória antes de sair
            return 1;
        }
    }

    // Ordena os processos (usando qsort, igual antes)
    qsort(p, n, sizeof(Processo), comparar_processos);

    // Calcula os tempos (lógica igual antes)
    // ... (código de cálculo dos tempos aqui) ...
    // (Exemplo resumido)
    for (i = 0; i < n; i++) {
        if (tempo_atual < p[i].chegada) {
            tempo_atual = p[i].chegada;
        }
        p[i].inicio = tempo_atual;
        p[i].termino = p[i].inicio + p[i].duracao;
        p[i].espera = p[i].inicio - p[i].chegada;
        p[i].retorno = p[i].termino - p[i].chegada;
        tempo_atual = p[i].termino;
        soma_espera += p[i].espera;
        soma_retorno += p[i].retorno;
    }


    // Exibe os resultados (lógica igual antes)
    // ... (código de exibição dos resultados aqui) ...
    printf("\n--- Resultados do Escalonamento FCFS ---\n");
    printf("+-----+---------+---------+--------+---------+--------+---------+\n");
    printf("| %-3s | %-7s | %-7s | %-6s | %-7s | %-6s | %-7s |\n",
           "ID", "Chegada", "Duração", "Início", "Término", "Espera", "Retorno");
    printf("+-----+---------+---------+--------+---------+--------+---------+\n");
    for (i = 0; i < n; i++) {
        printf("| %-3d | %-7d | %-7d | %-6d | %-7d | %-6d | %-7d |\n",
               p[i].id, p[i].chegada, p[i].duracao,
               p[i].inicio, p[i].termino, p[i].espera, p[i].retorno);
    }
    printf("+-----+---------+---------+--------+---------+--------+---------+\n");


    if (n > 0) {
        printf("\nTempo médio de espera: %.2f unidades de tempo\n", soma_espera / n);
        printf("Tempo médio de retorno: %.2f unidades de tempo\n", soma_retorno / n);
    }

    // Libera a memória alocada dinamicamente
    free(p);
    p = NULL; // Boa prática para evitar ponteiros "dangling"

    printf("\nSimulação FCFS concluída.\n");
    return 0;
}