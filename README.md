# Sistemas Operacionais - 2025.1

Este repositório reúne implementações de algoritmos clássicos de escalonamento de processos e problemas de concorrência, desenvolvidos como parte das atividades da disciplina de Sistemas Operacionais. Os códigos simulam e analisam diferentes estratégias de gerenciamento de processos e controle de concorrência, abordando conceitos fundamentais do funcionamento de sistemas operacionais modernos.

- **Disciplina**: Sistemas Operacionais
- **Semestre**: 2025.1
- **Professor**: Ary Henrique
- **Instituição**: CCOMP/UFT

## Projetos e Trabalhos

### Atividade 1: Concorrência e Processos

Implementações de exemplos clássicos de criação de processos e problemas do produtor-consumidor:

#### Acadêmicos: [Antonio André](https://github.com/andrebarceloschagas) e Ranor Victor

- **[pid.c](atividade_1/pid.c)**
  - **Descrição**: Exemplo de criação de processos com `fork()` e execução de comandos com `execlp`.
- **[produtor_consumidor_sem_controle.c](atividade_1/produtor_consumidor_sem_controle.c)**
  - **Descrição**: Produtor-consumidor sem nenhum tipo de controle de concorrência.
- **[produtor_consumidor_espera_ocupada.c](atividade_1/produtor_consumidor_espera_ocupada.c)**
  - **Descrição**: Produtor-consumidor com tentativa de controle usando espera ocupada.
- **[produtor_consumidor_tentativa.c](atividade_1/produtor_consumidor_tentativa.c)**
  - **Descrição**: Produtor-consumidor com controle de concorrência aprimorado, ainda baseado em espera ocupada.
- **[semaforo.c](atividade_1/semaforo.c)**
  - **Descrição**: Produtor-consumidor utilizando semáforos para controle correto da concorrência.

### Atividade 2: Algoritmos de Escalonamento

Implementações dos principais algoritmos de escalonamento de processos:

#### Acadêmicos: [Antonio André](https://github.com/andrebarceloschagas) e Luiz Felipe

- **[FCFS (First-Come, First-Served)](atividade_2/fcfs.c)**
  - **Descrição**: Simula o escalonamento onde o primeiro processo a chegar é o primeiro a ser executado.
- **[SJF (Shortest Job First)](atividade_2/sjf.c)**
  - **Descrição**: Simula o escalonamento pelo menor tempo de execução (não preemptivo).
- **[Round Robin](atividade_2/round_robin.c)**
  - **Descrição**: Simula o escalonamento com fatias de tempo (quantum), alternando entre os processos prontos.

## Organização

Os códigos estão organizados nas pastas `atividade_1` (concorrência e processos) e `atividade_2` (escalonamento), separados por algoritmo. Cada implementação solicita os dados dos processos via terminal e exibe os resultados detalhados, incluindo tempos de espera, retorno e médias.

## Como Executar

1. Acesse a pasta do algoritmo desejado.
2. Compile o arquivo `.c` com o GCC:
    ```bash
    gcc -o nome_do_programa nome_do_arquivo.c
    ```
3. Execute o programa:
    ```bash
    ./nome_do_programa
    ```
4. Siga as instruções exibidas no terminal para inserir os dados dos processos.

## Contribuições

Contribuições são bem-vindas! Se você deseja adicionar novos algoritmos, melhorar a documentação ou corrigir erros, sinta-se à vontade para abrir um pull request. Para sugestões ou dúvidas, entre em contato no email: antonio.andre@uft.edu.br.