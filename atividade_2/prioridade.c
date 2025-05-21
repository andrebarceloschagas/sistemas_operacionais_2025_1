


#include <stdio.h>
#include <stdlib.h> // Para malloc, free
#include <limits.h> // Para INT_MAX

// Estrutura que representa um processo
typedef struct {
    int id;                 // Identificador do processo
    int chegada;            // Tempo de chegada do processo
    int duracao_original;   // Duração original (tempo de execução total)
    int prioridade;         // Prioridade do processo (menor valor = maior prioridade)
    int restante;           // Tempo restante de execução para o processo
    int inicio;             // Momento em que o processo começa a executar pela primeira vez
    int termino;            // Momento em que o processo termina sua execução
    int espera;             // Tempo total de espera do processo
    int retorno;            // Tempo total de retorno (turnaround)
    int finalizado;         // Flag: 1 se o processo terminou, 0 caso contrário
} Processo;

int main() {
    Processo *p = NULL; // Ponteiro para o array de processos
    int n, i;
    int tempo_atual = 0;
    int processos_finalizados = 0;
    float soma_espera = 0, soma_retorno = 0;
    int ultimo_processo_executado = -1; // Para rastrear mudanças e registrar início

    printf("## Escalonamento por Prioridade Preemptivo ##\n");
    printf("-------------------------------------------\n");
    printf("(Menor valor numérico indica maior prioridade)\n\n");

    // Entrada do número de processos
    printf("Informe o número de processos: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        fprintf(stderr, "Erro: Número de processos deve ser um inteiro positivo.\n");
        return 1;
    }

    // Alocação dinâmica de memória para os processos
    p = (Processo *)malloc(n * sizeof(Processo));
    if (p == NULL) {
        fprintf(stderr, "Erro: Falha na alocação de memória para os processos!\n");
        return 1;
    }

    // Entrada dos dados dos processos
    printf("\n--- Entrada dos Dados dos Processos ---\n");
    for (i = 0; i < n; i++) {
        p[i].id = i + 1;
        printf("Processo %d:\n", p[i].id);
        printf("  Tempo de chegada: ");
        if (scanf("%d", &p[i].chegada) != 1 || p[i].chegada < 0) {
            fprintf(stderr, "Erro: Tempo de chegada inválido para o processo %d (deve ser >= 0).\n", p[i].id);
            free(p);
            return 1;
        }
        printf("  Duração (burst): ");
        if (scanf("%d", &p[i].duracao_original) != 1 || p[i].duracao_original <= 0) {
            fprintf(stderr, "Erro: Duração inválida para o processo %d (deve ser > 0).\n", p[i].id);
            free(p);
            return 1;
        }
        printf("  Prioridade: ");
        if (scanf("%d", &p[i].prioridade) != 1 || p[i].prioridade < 0) { // Prioridade pode ser 0
            fprintf(stderr, "Erro: Prioridade inválida para o processo %d (deve ser >= 0).\n", p[i].id);
            free(p);
            return 1;
        }
        p[i].restante = p[i].duracao_original;
        p[i].finalizado = 0;
        p[i].inicio = -1; // Indica que ainda não começou
        p[i].termino = 0;
        p[i].espera = 0;
        p[i].retorno = 0;
    }

    printf("\n--- Executando Escalonamento por Prioridade Preemptivo ---\n");

    // Loop principal: executa enquanto houver processos não finalizados
    while (processos_finalizados < n) {
        int idx_candidato = -1;
        int maior_prioridade_encontrada = INT_MAX; // Maior valor numérico (menor prioridade)

        // Seleciona o processo pronto com a maior prioridade (menor valor numérico)
        for (i = 0; i < n; i++) {
            if (!p[i].finalizado && p[i].chegada <= tempo_atual) {
                if (p[i].prioridade < maior_prioridade_encontrada) {
                    maior_prioridade_encontrada = p[i].prioridade;
                    idx_candidato = i;
                } else if (p[i].prioridade == maior_prioridade_encontrada) {
                    // Critério de desempate: menor tempo de chegada
                    if (p[i].chegada < p[idx_candidato].chegada) {
                        idx_candidato = i;
                    }
                    // Critério de desempate secundário: menor ID (se chegadas iguais)
                    else if (p[i].chegada == p[idx_candidato].chegada && p[i].id < p[idx_candidato].id) {
                        idx_candidato = i;
                    }
                }
            }
        }

        if (idx_candidato == -1) { // Nenhum processo pronto para executar agora
            if (processos_finalizados == n) break; // Todos os processos já terminaram

            int proxima_chegada_minima = INT_MAX;
            int encontrou_proximo_para_chegar = 0;
            // Avança o tempo para a chegada do próximo processo, se CPU estiver ociosa
            for(i=0; i<n; ++i) {
                if(!p[i].finalizado && p[i].chegada < proxima_chegada_minima) {
                    proxima_chegada_minima = p[i].chegada;
                    encontrou_proximo_para_chegar = 1;
                }
            }

            if(encontrou_proximo_para_chegar && proxima_chegada_minima > tempo_atual) {
                tempo_atual = proxima_chegada_minima;
            } else {
                tempo_atual++; // Apenas avança o tempo se não há salto definido
            }
            ultimo_processo_executado = -1; // CPU estava ociosa
            continue;
        }

        // Se o processo selecionado é diferente do último, ou se a CPU estava ociosa
        // e este é o primeiro a rodar, marca/atualiza seu tempo de início.
        if (p[idx_candidato].inicio == -1) {
            p[idx_candidato].inicio = tempo_atual;
        }
        // Se houve preempção ou a CPU estava ociosa e um novo processo começou
        if (ultimo_processo_executado != idx_candidato) {
             // Poderia imprimir aqui qual processo está rodando, se quisesse um log detalhado.
             // printf("Tempo %d: Processo %d está executando.\n", tempo_atual, p[idx_candidato].id);
             ultimo_processo_executado = idx_candidato;
        }


        // Executa o processo por uma unidade de tempo
        p[idx_candidato].restante--;
        tempo_atual++;

        // Verifica se o processo terminou
        if (p[idx_candidato].restante == 0) {
            p[idx_candidato].termino = tempo_atual;
            p[idx_candidato].retorno = p[idx_candidato].termino - p[idx_candidato].chegada;
            p[idx_candidato].espera = p[idx_candidato].retorno - p[idx_candidato].duracao_original;
            p[idx_candidato].finalizado = 1;
            processos_finalizados++;
            ultimo_processo_executado = -1; // Processo terminou, o próximo será uma "nova" escolha

            // Acumula para cálculo das médias
            soma_espera += p[idx_candidato].espera;
            soma_retorno += p[idx_candidato].retorno;
        }
    }

    // Exibe os resultados
    printf("\n--- Resultados do Escalonamento por Prioridade Preemptivo ---\n");
    printf("+-----+---------+---------+------------+--------+---------+--------+---------+\n");
    printf("| %-3s | %-7s | %-7s | %-10s | %-6s | %-7s | %-6s | %-7s |\n",
           "ID", "Chegada", "Duração", "Prioridade", "Início", "Término", "Espera", "Retorno");
    printf("+-----+---------+---------+------------+--------+---------+--------+---------+\n");

    for (i = 0; i < n; i++) {
        printf("| %-3d | %-7d | %-7d | %-10d | %-6d | %-7d | %-6d | %-7d |\n",
               p[i].id, p[i].chegada, p[i].duracao_original, p[i].prioridade,
               p[i].inicio, p[i].termino, p[i].espera, p[i].retorno);
    }
    printf("+-----+---------+---------+------------+--------+---------+--------+---------+\n");

    if (n > 0) {
        printf("\nTempo médio de espera: %.2f unidades de tempo\n", soma_espera / n);
        printf("Tempo médio de retorno: %.2f unidades de tempo\n", soma_retorno / n);
    }
    printf("\nSimulação concluída.\n");

    // Liberação da memória dinâmica
    if (p != NULL) {
        free(p);
        p = NULL;
    }

    return 0;
}