/*
 * Algoritmo de Escalonamento SJF (Shortest Job First)
 * Este programa simula o escalonamento de processos pelo critério do menor trabalho primeiro (menor duração).
 * Cada processo possui tempo de chegada, duração, e o programa calcula tempos de início, término, espera e retorno.
 * Autor: Antonio André Barcelos Chagas
 * Data: 17/05/2025
 *
 * Variáveis principais:
 * - p[MAX]: vetor de processos
 * - n: quantidade de processos
 * - tempo_atual: controla o tempo global de execução
 * - processos_finalizados: conta quantos processos já foram concluídos
 * - soma_espera, soma_retorno: acumuladores para médias
 */

 #include <stdio.h>
 #include <stdlib.h> // Para malloc, free, exit
 
 // Estrutura que representa um processo
 typedef struct {
     int id;         // Identificador do processo
     int chegada;    // Tempo de chegada do processo
     int duracao;    // Duração (tempo de execução) do processo
     int inicio;     // Momento em que o processo começa a executar
     int termino;    // Momento em que o processo termina
     int espera;     // Tempo de espera na fila
     int retorno;    // Tempo de retorno (turnaround)
     int finalizado; // Indica se o processo já foi finalizado (1) ou não (0)
 } Processo;
 
 int main() {
     Processo *p = NULL; // Ponteiro para o array de processos (para alocação dinâmica)
     int n, i;
     int tempo_atual = 0;
     int processos_finalizados = 0;
     float soma_espera = 0, soma_retorno = 0;
 
     printf("Algoritmo de Escalonamento SJF (Shortest Job First) Não Preemptivo\n");
     printf("------------------------------------------------------------------\n");
 
     // Entrada do número de processos
     printf("Informe o número de processos: ");
     if (scanf("%d", &n) != 1 || n <= 0) {
         fprintf(stderr, "Erro: Número de processos deve ser um inteiro positivo.\n");
         return 1;
     }
 
     // --- ALOCAÇÃO DINÂMICA DE MEMÓRIA ---
     p = (Processo *)malloc(n * sizeof(Processo));
     if (p == NULL) {
         fprintf(stderr, "Erro: Falha na alocação de memória para os processos!\n");
         return 1;
     }
     // --- FIM DA ALOCAÇÃO DINÂMICA ---
 
     // Entrada dos dados dos processos
     printf("\n--- Entrada dos Dados dos Processos ---\n");
     for (i = 0; i < n; i++) {
         p[i].id = i + 1;
         printf("Processo %d:\n", p[i].id);
         printf("  Tempo de chegada: ");
         if (scanf("%d", &p[i].chegada) != 1 || p[i].chegada < 0) {
             fprintf(stderr, "Erro: Tempo de chegada inválido para o processo %d (deve ser >= 0).\n", p[i].id);
             free(p); // Libera memória alocada antes de sair
             return 1;
         }
         printf("  Duração (burst): ");
         if (scanf("%d", &p[i].duracao) != 1 || p[i].duracao <= 0) {
             fprintf(stderr, "Erro: Duração inválida para o processo %d (deve ser > 0).\n", p[i].id);
             free(p); // Libera memória alocada antes de sair
             return 1;
         }
         p[i].finalizado = 0; // Inicialmente, nenhum processo está finalizado
         p[i].inicio = -1;    // Inicializa para indicar que não começou
         p[i].termino = -1;
     }
 
     printf("\n--- Executando Escalonamento SJF Não Preemptivo ---\n");
 
     // Loop principal: executa até todos os processos serem finalizados
     while (processos_finalizados < n) {
         int idx_selecionado = -1;   // Índice do processo selecionado para execução
         int menor_duracao_atual = -1; // Menor duração encontrada entre os processos prontos
 
         // Busca o processo disponível (chegou e não finalizado) com menor duração
         for (i = 0; i < n; i++) {
             if (!p[i].finalizado && p[i].chegada <= tempo_atual) {
                 if (idx_selecionado == -1 || p[i].duracao < menor_duracao_atual) {
                     menor_duracao_atual = p[i].duracao;
                     idx_selecionado = i;
                 }
                 // Critério de desempate (opcional): se durações iguais, menor tempo de chegada
                 else if (p[i].duracao == menor_duracao_atual) {
                     if (p[i].chegada < p[idx_selecionado].chegada) {
                         idx_selecionado = i;
                     }
                     // Outro desempate (opcional): se chegada também igual, menor ID
                     else if (p[i].chegada == p[idx_selecionado].chegada && p[i].id < p[idx_selecionado].id) {
                          idx_selecionado = i;
                     }
                 }
             }
         }
 
         // Se nenhum processo está pronto (CPU Ociosa)
         if (idx_selecionado == -1) {
             if (processos_finalizados == n) { // Todos já finalizaram, sair do loop
                 break;
             }
             int proxima_chegada_minima = -1;
             int encontrou_proximo = 0;
 
             // Avança o tempo para o próximo processo que irá chegar
             for (i = 0; i < n; i++) {
                 if (!p[i].finalizado) { // Considera apenas os não finalizados
                     if (!encontrou_proximo || p[i].chegada < proxima_chegada_minima) {
                         proxima_chegada_minima = p[i].chegada;
                         encontrou_proximo = 1;
                     }
                 }
             }
 
             if (encontrou_proximo) { // Se há processos futuros
                 if (tempo_atual < proxima_chegada_minima) { // Avança o tempo se a chegada for no futuro
                     tempo_atual = proxima_chegada_minima;
                 } else {
                     // Se o próximo a chegar já chegou ou está no tempo atual,
                     // mas não foi selecionado, algo está estranho (idx_selecionado deveria ter sido definido).
                     // Para garantir progresso caso a lógica de seleção tenha uma condição limite,
                     // avançamos o tempo minimamente.
                     tempo_atual++;
                 }
             } else {
                 // Nenhum processo não finalizado restante. Isso significa que processos_finalizados == n.
                 // O loop while deve terminar. Se não, é um erro de estado.
                 if (processos_finalizados < n) {
                    fprintf(stderr, "Alerta: CPU ociosa, mas há processos não finalizados sem chegadas futuras.\n");
                     // Isso pode indicar um problema se o loop não terminar.
                     // Por segurança, apenas avançar o tempo para tentar novamente.
                     tempo_atual++;
                 } else {
                     break; // Todos os processos foram contabilizados como finalizados.
                 }
             }
             continue; // Volta ao início do loop para reavaliar com o novo tempo_atual
         }
 
         // Executa o processo selecionado
         // Garante que o tempo de início não é anterior à chegada (embora a seleção já deva cuidar disso)
         if (tempo_atual < p[idx_selecionado].chegada) {
             tempo_atual = p[idx_selecionado].chegada;
         }
         p[idx_selecionado].inicio = tempo_atual;
         p[idx_selecionado].termino = p[idx_selecionado].inicio + p[idx_selecionado].duracao;
         p[idx_selecionado].espera = p[idx_selecionado].inicio - p[idx_selecionado].chegada;
         p[idx_selecionado].retorno = p[idx_selecionado].termino - p[idx_selecionado].chegada;
 
         tempo_atual = p[idx_selecionado].termino; // Atualiza o tempo global
         p[idx_selecionado].finalizado = 1;
         processos_finalizados++;
 
         // Acumula para cálculo das médias
         soma_espera += p[idx_selecionado].espera;
         soma_retorno += p[idx_selecionado].retorno;
     }
 
     // Exibe os resultados
     printf("\n--- Resultados do Escalonamento SJF Não Preemptivo ---\n");
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
 
     if (n > 0) { // Evita divisão por zero
         printf("\nTempo médio de espera: %.2f unidades de tempo\n", soma_espera / n);
         printf("Tempo médio de retorno: %.2f unidades de tempo\n", soma_retorno / n);
     }
     printf("\nSimulação SJF Não Preemptivo concluída.\n");
 
     // --- LIBERAÇÃO DA MEMÓRIA DINÂMICA ---
     if (p != NULL) {
         free(p);
         p = NULL; // Boa prática
     }
     // --- FIM DA LIBERAÇÃO ---
 
     return 0;
 }
 