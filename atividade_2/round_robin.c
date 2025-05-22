/* Algoritmo de Escalonamento Round Robin (RR)
 * Este programa simula o escalonamento de processos pelo critério Round Robin, onde cada processo recebe um quantum de tempo para execução.
 * O algoritmo gerencia a fila de processos prontos, alternando entre eles até que todos sejam finalizados.
 * Autor: Antonio André Barcelos Chagas
 * Data: 17/05/2025
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
 #include <stdlib.h> // Para exit() em caso de erro grave
 
 #define MAX 100 // Define o número máximo de processos
 
 // Estrutura que representa um processo
 typedef struct {
     int id;          // Identificador do processo
     int chegada;     // Tempo de chegada do processo
     int duracao;     // Duração original (tempo de execução total) do processo
     int restante;    // Tempo restante de execução para o processo
     int inicio;      // Momento em que o processo começa a executar pela primeira vez
     int termino;     // Momento em que o processo termina sua execução
     int espera;      // Tempo total de espera do processo
     int retorno;     // Tempo total de retorno (turnaround) do processo
     int finalizado;  // Flag: 1 se o processo terminou, 0 caso contrário
     // int em_fila; // Mantido implicitamente pela lógica do array 'fila_prontos' e 'foi_para_fila'
 } Processo;
 
 int main() {
     Processo p[MAX];
     int n, i, quantum;
     int tempo_atual = 0;
     int processos_finalizados = 0;
     float soma_espera = 0, soma_retorno = 0;
 
     printf("Algoritmo de Escalonamento Round Robin\n");
     printf("-------------------------------------\n");
 
     // Entrada do número de processos
     printf("Informe o número de processos (max %d): ", MAX);
     if (scanf("%d", &n) != 1) {
         fprintf(stderr, "Erro: Entrada inválida para o número de processos.\n");
         return 1;
     }
     if (n <= 0 || n > MAX) {
         printf("Número de processos deve ser positivo e menor ou igual a %d.\n", MAX);
         if (n <= 0) printf("Nenhum processo para escalonar.\n");
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
             return 1;
         }
         printf("  Duração (burst): ");
         if (scanf("%d", &p[i].duracao) != 1 || p[i].duracao <= 0) {
             fprintf(stderr, "Erro: Duração inválida para o processo %d (deve ser > 0).\n", p[i].id);
             return 1;
         }
         p[i].restante = p[i].duracao; // Tempo restante é inicialmente a duração total
         p[i].inicio = -1;             // Marca que o processo ainda não iniciou (-1 é um valor sentinela)
         p[i].finalizado = 0;          // Marca que o processo não foi finalizado
         p[i].termino = -1;            // Inicializa tempos de resultado
         p[i].espera = 0;
         p[i].retorno = 0;
     }
 
     // Entrada do quantum
     printf("\nInforme o valor do quantum: ");
     if (scanf("%d", &quantum) != 1 || quantum <= 0) {
         fprintf(stderr, "Erro: O valor do quantum deve ser um inteiro positivo.\n");
         return 1;
     }
 
     int fila_prontos[MAX]; // Fila para os índices dos processos prontos
     int inicio_fila = 0;
     int fim_fila = 0;
     // Flag para controlar se um processo já foi adicionado à fila de prontos (para evitar duplicatas desnecessárias)
     // e para ajudar na lógica de CPU ociosa.
     // 0 = não considerado para fila ainda, 1 = já entrou no sistema de fila/execução.
     int entrou_no_sistema[MAX] = {0};
 
     printf("\n--- Executando Escalonamento Round Robin ---\n");
 
     // Loop principal: executa enquanto houver processos não finalizados
     while (processos_finalizados < n) {
         int processos_adicionados_nesta_iteracao = 0;
 
         // 1. Adicionar processos à fila de prontos
         // Adiciona processos que chegaram (chegada <= tempo_atual) e ainda não entraram no sistema.
         for (i = 0; i < n; i++) {
             if (!p[i].finalizado && !entrou_no_sistema[i] && p[i].chegada <= tempo_atual) {
                 if (fim_fila < MAX) { // Verifica se a fila tem espaço (segurança)
                     fila_prontos[fim_fila++] = i;
                     entrou_no_sistema[i] = 1; // Marca que o processo entrou no sistema de enfileiramento
                     processos_adicionados_nesta_iteracao++;
                 } else {
                     fprintf(stderr, "Erro: Fila de prontos excedeu a capacidade máxima. Aumente MAX.\n");
                     return 1;
                 }
             }
         }
 
         // 2. Se a fila de prontos estiver vazia (CPU Ociosa)
         if (inicio_fila == fim_fila) {
             if (processos_finalizados == n) { // Todos os processos terminaram
                 break;
             }
 
             // Encontrar o menor tempo de chegada futuro de um processo que ainda não entrou no sistema
             int menor_chegada_futura = -1;
             for (i = 0; i < n; i++) {
                 if (!p[i].finalizado && !entrou_no_sistema[i]) {
                     if (menor_chegada_futura == -1 || p[i].chegada < menor_chegada_futura) {
                         menor_chegada_futura = p[i].chegada;
                     }
                 }
             }
 
             if (menor_chegada_futura != -1) { // Se existe um processo futuro para chegar
                 if (tempo_atual < menor_chegada_futura) {
                     tempo_atual = menor_chegada_futura; // Avança o tempo para a chegada do próximo
                 } else {
                      // Processo já deveria ter chegado ou está chegando agora, mas não entrou no sistema
                      // O loop de adição no início da próxima iteração do while deve pegá-lo.
                      // Apenas incrementa o tempo para garantir progresso se não houver avanço para chegada futura.
                      tempo_atual++;
                 }
             } else {
                 // Fila vazia, nenhum processo novo para entrar no sistema, mas nem todos finalizaram.
                 // Isso pode ocorrer se todos os processos restantes já estão "entrou_no_sistema = 1"
                 // mas não estão na fila (o que seria um erro de estado) ou se o loop de finalização
                 // não está correto. Por segurança, apenas avança o tempo se há trabalho a fazer.
                 if (processos_finalizados < n) {
                     tempo_atual++;
                 } else {
                     break; // Segurança, loop principal deve pegar
                 }
             }
             continue; // Volta ao início do loop para reavaliar com o novo tempo_atual
         }
 
         // 3. Retirar o próximo processo da fila de prontos
         int idx_processo_atual = fila_prontos[inicio_fila++];
 
         // Registrar o tempo de início se for a primeira vez que o processo executa
         if (p[idx_processo_atual].inicio == -1) {
             p[idx_processo_atual].inicio = tempo_atual;
         }
 
         // 4. Executar o processo pelo quantum ou pelo tempo restante
         int tempo_de_execucao_nesta_fatia;
         if (p[idx_processo_atual].restante <= quantum) {
             tempo_de_execucao_nesta_fatia = p[idx_processo_atual].restante;
             p[idx_processo_atual].finalizado = 1; // Processo terminará nesta fatia
         } else {
             tempo_de_execucao_nesta_fatia = quantum;
         }
 
         tempo_atual += tempo_de_execucao_nesta_fatia;
         p[idx_processo_atual].restante -= tempo_de_execucao_nesta_fatia;
 
         // 5. Adicionar processos que chegaram DURANTE a execução desta fatia
         // (Esta lógica já está no início do loop while, que é mais abrangente
         // e será executada na próxima iteração, considerando o novo tempo_atual.
         // Repetir aqui seria redundante se a estrutura do loop principal estiver correta.)
         // A estrutura atual (adicionar no início do while, depois pegar da fila) é comum.
 
         // 6. Lidar com o processo após sua fatia de tempo
         if (p[idx_processo_atual].finalizado) {
             p[idx_processo_atual].termino = tempo_atual;
             p[idx_processo_atual].retorno = p[idx_processo_atual].termino - p[idx_processo_atual].chegada;
             p[idx_processo_atual].espera = p[idx_processo_atual].retorno - p[idx_processo_atual].duracao;
             processos_finalizados++;
         } else {
             // Processo não terminou, precisa voltar para o fim da fila de prontos.
             // Adicionar processos que chegaram *antes* de re-enfileirar o atual é importante
             // para manter a ordem correta de chegada.
             // O loop no início do while já cuida da adição de novos processos.
             // Agora, re-enfileiramos o processo atual.
             if (fim_fila < MAX) {
                  fila_prontos[fim_fila++] = idx_processo_atual;
             } else {
                 fprintf(stderr, "Erro: Fila de prontos excedeu a capacidade máxima ao tentar re-enfileirar. Aumente MAX.\n");
                 return 1;
             }
         }
     }
 
     // 7. Exibir os resultados
     printf("\n--- Resultados do Escalonamento Round Robin (Quantum: %d) ---\n", quantum);
     printf("+-----+---------+---------+--------+---------+--------+---------+\n");
     printf("| %-3s | %-7s | %-7s | %-6s | %-7s | %-6s | %-7s |\n",
            "ID", "Chegada", "Duração", "Início", "Término", "Espera", "Retorno");
     printf("+-----+---------+---------+--------+---------+--------+---------+\n");
 
     for (i = 0; i < n; i++) {
         printf("| %-3d | %-7d | %-7d | %-6d | %-7d | %-6d | %-7d |\n",
                p[i].id, p[i].chegada, p[i].duracao,
                p[i].inicio, p[i].termino, p[i].espera, p[i].retorno);
         soma_espera += p[i].espera;
         soma_retorno += p[i].retorno;
     }
     printf("+-----+---------+---------+--------+---------+--------+---------+\n");
 
     if (n > 0) {
         printf("\nTempo médio de espera: %.2f unidades de tempo\n", soma_espera / n);
         printf("Tempo médio de retorno: %.2f unidades de tempo\n", soma_retorno / n);
     }
     printf("\nSimulação Round Robin concluída.\n");
 
     return 0;
 }
