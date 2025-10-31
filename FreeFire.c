#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// Código da Ilha ? Edição Free Fire
// Nível: Mestre
// Este programa simula o gerenciamento avançado de uma mochila com componentes coletados durante a fuga de uma ilha.

// Definições fixa do tamanho durante a execução do programa nome e tipo do item
#define TAM_NOME 30
#define TAM_TIPO 20

// Define uma contante para o número máximo de itens que podemos ter
#define TAM_MAX 10 

// Struct Item:
// Representa um componente com nome, tipo, quantidade e prioridade.
// Agora vamos criar nossa estrutura Item com o comando struct 
// Representando as informações de 1 item
typedef struct {
    char nome[TAM_NOME];
    char tipo[TAM_TIPO];
    int quantidade; 
    int prioridade;
} Item;

// Estrutura que representa o inventário completo, com uma variavel que representa o contador de itens no Inventario
typedef struct {
    Item itens[TAM_MAX];
    int numeroItens;
    // Variavel status da ordenação por Nome
    char status[TAM_NOME];
} Inventario;

// Função de troca (utilizada pelos algoritmos de ordenação)
void trocarItens(Item *a, Item *b) {
    Item temp = *a;
    *a = *b;
    *b = temp;
}

// ----------------------------------------------------------------------
// FUNÇÕES DE ORDENAÇÃO (COM CONTADOR DE COMPARAÇÕES)
// ----------------------------------------------------------------------

// Função de comparação para os critérios de ordenação
// Retorna <0, 0, ou >0 dependendo da ordem.
int compararItens(const Item *a, const Item *b, int criterio, long int *comparacoes) {
    (*comparacoes)++; // Contabiliza UMA comparação
    if (criterio == 1) { // Por Nome
        return strcmp(a->nome, b->nome);
    } else if (criterio == 2) { // Por Tipo
        return strcmp(a->tipo, b->tipo);
    } else if (criterio == 3) { // Por Prioridade
        // Prioridade decrescente (maior prioridade vem primeiro)
        if (a->prioridade < b->prioridade) return 1;
        if (a->prioridade > b->prioridade) return -1;
        return 0; // Prioridades iguais
    }
    return 0; // Critério inválido
}

// Implementação do Bubble Sort
void bubbleSort(Inventario *inv, int criterio, long int *comparacoes) {
    int n = inv->numeroItens;
    *comparacoes = 0; // Zera o contador
    for (int i = 0; i < n - 1; i++) {
        bool trocado = false;
        for (int j = 0; j < n - i - 1; j++) {
            // A comparação está dentro da função 'compararItens'
            if (compararItens(&(inv->itens[j]), &(inv->itens[j + 1]), criterio, comparacoes) > 0) {
                trocarItens(&(inv->itens[j]), &(inv->itens[j + 1]));
                trocado = true;
            }
        }
        if (!trocado) break; // Otimização para vetores já ordenados
    }
}

// Implementação do Insertion Sort
void insertionSort(Inventario *inv, int criterio, long int *comparacoes) {
    int n = inv->numeroItens;
    *comparacoes = 0; // Zera o contador
    for (int i = 1; i < n; i++) {
        Item chave = inv->itens[i];
        int j = i - 1;
        // O while é a parte crítica de comparação
        while (j >= 0 && compararItens(&(inv->itens[j]), &chave, criterio, comparacoes) > 0) {
            inv->itens[j + 1] = inv->itens[j];
            j = j - 1;
        }
        inv->itens[j + 1] = chave;
        // Se o loop while não rodou (j < 0), a primeira condição do while ainda contou uma comparação
        // Se o loop rodou pelo menos uma vez, a última comparação (que fez a condição ser falsa)
        // já foi contabilizada na iteração anterior do while.
        // A lógica do contar comparações dentro da função compararItens simplifica a contagem.
    }
    // O Insertion Sort anterior no seu código já era funcional e contava 
    // implicitamente as comparações. Mantenho a nova estrutura para usar a função
    // unificada de comparação e contagem.
}

// Implementação do Selection Sort
void selectionSort(Inventario *inv, int criterio, long int *comparacoes) {
    int n = inv->numeroItens;
    *comparacoes = 0; // Zera o contador
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            // A comparação está dentro da função 'compararItens'
            if (compararItens(&(inv->itens[j]), &(inv->itens[min_idx]), criterio, comparacoes) < 0) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            trocarItens(&(inv->itens[i]), &(inv->itens[min_idx]));
        }
    }
}

// Função para limpar o buffer de entrada
void liberarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Função para remover a quebra de linha (\n) lida pelo fgets
void limpa_quebra_linha(char *string) {
    string[strcspn(string, "\n")] = '\0';
}

// Função para limpar a tela do terminal após o ENTER do usuario
void limpar_tela() {
    // Verifica se o SO é Windows (MSVC)
    #ifdef _WIN32
        system("cls");
    // Caso contrário, assume-se sistemas baseados em Unix (Linux, macOS, etc.)
    #else
        system("clear");
    #endif
}

// --- Funções principais ---
// Função para inserir um item
void inseriritem(Inventario *inv);

// Função para remover um item
void removeritem(Inventario *inv);

// Função para listar um item
void listarItens(const Inventario *inv);

// Função Buscar item por nome
void buscaritem(Inventario *inv);

// Função Ordenar componentes
void ordenarComponente(Inventario *inv);

// Função que apresenta o menu de métodos de ordenação
void menuOrdenacao(Inventario *inv, int criterio) {
    int escolhaMetodo;
    long int comparacoes = 0;

    printf("\n--- Escolha do Algoritmo de Ordenação ---\n");
    printf("1. Bubble Sort\n");
    printf("2. Insertion Sort\n");
    printf("3. Selection Sort\n");
    printf("0. Cancelar\n");
    printf("Escolha o método de ordenação: ");
    
    while (scanf("%d", &escolhaMetodo) != 1 || escolhaMetodo > 3 || escolhaMetodo < 0) {
        liberarBuffer();
        printf("ERRO: Escolha inexistente! Escolha novamente: ");
    }
    liberarBuffer();
    printf("\n");

    if (escolhaMetodo == 0) return;

    // Tratamento de inventário vazio
    if (inv->numeroItens == 0) {
        printf("Inventário vazio. Nada para ordenar.\n");
        return;
    }

    switch (escolhaMetodo) {
        case 1:
            bubbleSort(inv, criterio, &comparacoes);
            printf("Ordenação por Bubble Sort concluída!\n");
            break;
        case 2:
            insertionSort(inv, criterio, &comparacoes);
            printf("Ordenação por Insertion Sort concluída!\n");
            break;
        case 3:
            selectionSort(inv, criterio, &comparacoes);
            printf("Ordenação por Selection Sort concluída!\n");
            break;
    }

    // Renomeia o status da ordenação por nome
    if (criterio == 1) {
        strcpy(inv->status, "ORDENADO");
    } else {
        strcpy(inv->status, "NAO ORDENADO");
    }
    
    // ANÁLISE DE DESEMPENHO
    printf("========================================================\n");
    printf("ANÁLISE DE DESEMPENHO (%s)\n", (escolhaMetodo == 1) ? "Bubble Sort" : (escolhaMetodo == 2) ? "Insertion Sort" : "Selection Sort");
    printf("Itens Ordenados: %d\n", inv->numeroItens);
    printf("Total de Comparações Realizadas: %ld\n", comparacoes);
    printf("========================================================\n");
}


// A função ordenarComponente agora gerencia apenas o critério (Nome/Tipo/Prioridade)
// e chama a função menuOrdenacao para escolher o algoritmo.
void ordenarComponente(Inventario *inv){
    printf("--- Estrategia de Organização ---\n");
    printf("1. Por Nome (Ordem Alfabética) \n");
    printf("2. Por Tipo \n");
    printf("3. Por Prioridade de Montagem (Decrescente) \n");
    printf("0. Cancelar \n");
    printf("Escolha o critério: ");

    int criterio;

    while (scanf("%d", &criterio) != 1 || criterio > 3 || criterio < 0 ) {
        liberarBuffer(); 
        printf("ERRO: Escolha inexistente! Escolha novamente: ");
    }
    liberarBuffer();
    
    if (criterio == 0) return;

    menuOrdenacao(inv, criterio);
}

int main() {
    
    // Criação da variável da struct Inventario
    Inventario inventario;

    // Inicializador do contador de itens começando com zero
    inventario.numeroItens = 0;

    // Inicializar a variavel status da ordenação por Nome com NAO ORDENADO
    strcpy(inventario.status, "NAO ORDENADO");

    // Variavel inteiro para representar a escolha do usuario
    // Será usada na estrutura switch
    int opcao;

    do { 
        printf("\n");      
        printf("====================================================="); 
        printf("\n   PLANO DE FUGA - CODIGO DA ILHA (NIVEL MESTRE) \n");
        printf("===================================================\n"); 

        // Representação da quantidade de itens na mochila
        // Opções de escolha do usuario representando por número inteiro que pode digitar
        printf("Itens na Mochila: %d/10\n",inventario.numeroItens);  
        printf("Status da ordenação por Nome: %s \n",inventario.status);
        printf("\n");        
        printf("1. Adicionar Componente \n");        
        printf("2. Descartar Componente \n");        
        printf("3. Listar Componentes (Inventario) \n"); 
        printf("4. Organizar Mochila (Ordenar Componentes) \n");
        printf("5. Busca Binaria por Componente-Chave (por Nome) \n");        
        printf("0. ATIVAR TORRE DE FUGA (Sair) \n");
        printf("-----------------------------\n");
        printf("Escolha uma opção: ");

        // Loop para garantir uma entrada válida para escolha
        // Se o usuario digitar algoi diferente de um numero inteiro
        // Ou digitar um numero maior que o numero de opções (3)
        // Ou digitar um numero menor que o numero de opções (0)
        // O programa notificará o erro
        while (scanf("%d", &opcao) != 1 || opcao > 5 || opcao < 0 ) {
            liberarBuffer(); // Limpa o buffer
            printf("ERRO: Opção inexistente! Escolha novamente: ");
        }
        liberarBuffer();
        printf("\n");

        // Estrutura switch com as opçoes de case de acordo com a entrada do usuario
        switch (opcao) {
            case 1:     
                printf(" --- Adicionar Novo Item --- \n");

                // Aqui chamamos as funções de acordo com o endereço de memoria de onde os itens serão inseridos
                inseriritem(&inventario);
                printf("Pressione ENTER para continuar... ");
                // Comando que espera o usuario para prosseguir a execução do programa com o ENTER
                getchar();
                limpar_tela();
                break;
            case 2:
                removeritem(&inventario);
                break;
            case 3:
                listarItens(&inventario);
                break;
            case 4:
                limpar_tela();
                ordenarComponente(&inventario);
                break;
            case 5:
                buscaritem(&inventario);
                printf("Pressione ENTER para continuar... ");
                // Comando que espera o usuario para prosseguir a execução do programa com o ENTER
                getchar();
                limpar_tela();
                break;
            break;
            case 0:
                printf("Saindo...\n");
                break;
        }

        // A estrutura switch trata cada opção chamando a função correspondente.
        // Quando a entrada do usuario for igual a zero, o sistema sai da estrutura for
    } while (opcao != 0); 

    return 0;
}

// Implementação das funções:

// inserirItem():
// Adiciona um novo componente à mochila se houver espaço.
// Solicita nome, tipo, quantidade

void inseriritem(Inventario *inv) {
    // Tratamento de erro caso o inventario já atingir o numero maxima de itens 10
    if (inv->numeroItens >= TAM_MAX) {
        printf("Erro: inventario cheio!\n");
        return;
    }

    // Variável da struct item, onde um novo item será inserido
    Item novo;
    printf("Nome do Componente: ");

    // Comando fgets para extrair a entrada do usuario
    fgets(novo.nome, TAM_NOME, stdin);

    // O fgets insere uma linha após a execução, logo, esta função excluirá esta linha
    limpa_quebra_linha(novo.nome);

    printf("Tipo (Estrutural, Eletronico, Energia): ");
    fgets(novo.tipo, TAM_TIPO, stdin);
    limpa_quebra_linha(novo.tipo);

    printf("Quantidade: ");
    scanf("%d", &novo.quantidade);
    liberarBuffer();

    printf("Prioridade de Montagem (1-5): ");
    scanf("%d", &novo.prioridade);
    liberarBuffer();

    // Atribui o novo item ao indice de itens
    inv->itens[inv->numeroItens] = novo;

    // Atualiza a quantidade de itens dentro do inventario
    inv->numeroItens++;
    printf("\n");
    printf("Componente \"%s\" adicionado com sucesso!\n", novo.nome);
}

// removerItem():
// Permite remover um componente da mochila pelo nome.
// Se encontrado, reorganiza o vetor para preencher a lacuna.
void removeritem(Inventario *inv) {

    // Tratamento de erros para o caso de o inventario não ter itens para excluir
    if (inv->numeroItens == 0) {
        printf("Inventario vazio!\n");
        return;
    }

    // variavel com a entrada do usuário do item que devemos remover
    char nomeRemover[TAM_NOME];
    printf("Digite o nome do componente a remover: ");
    fgets(nomeRemover, TAM_NOME, stdin);
    limpa_quebra_linha(nomeRemover);

    // Estrutura for para percorrer os itens até encontrar o que tem o mesmo nome que o usuario quer remover
    // Inicialização da posição do itens para que após encontrar a posição ser gravada dentro do if onde faz a comparações dos itens
    // Se os nomes forem iguais, o comando strcmp retorna O
    int pos = -1;
    for (int i = 0; i < inv->numeroItens; i++) {
        if (strcmp(inv->itens[i].nome, nomeRemover) == 0) {
            pos = i;
            break;
        }
    }

    // Caso a função não encontre o item com o nome igual ao do usuario
    // A pos continuará sendo o inicializou, logo, o item não foi encontrado
    if (pos == -1) {
        printf("Erro: Componente \"%s\" nao encontrado.\n", nomeRemover);
        return;
    }

    // Estrutura que percorre os itens atualizando suas posições
    // Porque caso um item seja removido, o indice das variaveis itens do inventario tem que ser atualizado
    for (int i = pos; i < inv->numeroItens - 1; i++) {
        inv->itens[i] = inv->itens[i + 1];
    }

    // Atualização da quantidade de intens dentro de inventario
    inv->numeroItens--;
    printf("Componente \"%s\" removido com sucesso.\n", nomeRemover);
}

// listarItens():
// Exibe uma tabela formatada com todos os componentes presentes na mochila.
void listarItens(const Inventario *inv) {

    // Tratamento de erros para o caso de o inventario não ter itens para listar
    if (inv->numeroItens == 0) {
        printf("O inventario esta vazio.\n");
        return;
    }

    printf("\n--- INVENTARIO ATUAL (%d/%d) ---\n", inv->numeroItens, TAM_MAX);
    printf("-------------------------------------------------------------\n");
    printf("NOME                       | TIPO           | QUANTIDADE     | PRIORIDADE\n");
    printf("-------------------------------------------------------------\n");

    // A estrutura for percorre o itens no inventario para listá-los
    // Após a porcentagem temos o numero de espaços que serão alocados com o conteudo das caracteristicas de cada itens
    for (int i = 0; i < inv->numeroItens; i++) {
        printf("%-26s | %-14s | %-4d | %3d \n",
               inv->itens[i].nome,
               inv->itens[i].tipo,
               inv->itens[i].quantidade,
               inv->itens[i].prioridade);
    }
    printf("-------------------------------------------------------------\n");
}


void buscaritem(Inventario *inv) {
    // Tratamento de erros para o caso de o inventario não ter itens para buscar
    if (inv->numeroItens == 0) {
        printf("Inventario vazio!\n");
        return;
    }

    // Tratamento se está ordenado por nome, senão a busca binario nao funciona
    if (strcmp(inv->status, "NAO ORDENADO") == 0) {
        printf("ALERTA: A busca binária requer que a mochila esteja ordenada por NOME. \n");
        printf("Use a Opcao 4 para organizar a mochila primeiro. \n");
        return;
    }

    // variavel com a entrada do usuário do item que queremos buscar
    char nomeItem[TAM_NOME];
    printf("--- Busca Binaria por Componente-Chave --- \n");
    printf("Nome do componente a buscar: ");
    fgets(nomeItem, TAM_NOME, stdin);
    limpa_quebra_linha(nomeItem);

    // Variável de inicialização da posição, quando encontrar o item esta variavel terá a posição do item desejado
    int pos = -1;

    // Variavel do loop while, que vai controlar a condição para que o loop percorra o array
    int inicio = 0, fim = inv->numeroItens - 1;

    // O WHILE irá percorrer sequencialmente todo o vetor
    while (inicio <= fim) { 
        // Declaração da variavel que determinará o indice do meio     
        int meio = (inicio + fim) / 2;
        int cmp = strcmp(inv->itens[meio].nome, nomeItem);

        // COmando strcmp vai comparar o indice vetor com o nome do item desejado
        // Encontrando, mudará a variavel meio, atribuindo o indice do item desejado
        if (cmp == 0) {
            pos = meio;
            break;
        } else if (cmp < 0) {
            inicio = meio + 1; 
        } else {
            fim = meio - 1;
        }
    }

    // Caso a função não encontre o item com o nome igual ao do usuario
    // A pos continuará sendo a que inicializou, logo, o item não foi encontrado
    if (pos == -1) {
        printf("Resultado: Componente-chave \"%s\" NÃO foi encontrado na mochila.\n", nomeItem);
        return;
    }

    // A partir do momento que o item for encontrado
    // Este printf imprimir o nome, tipo e quantidade do item desejado
    printf("\n--- Componente-chave Encontrado! --- \n");
    printf("Nome: \"%s\", Tipo: \"%s\", Qtd: \"%d\", Prio: \"%d\" \n", 
        inv->itens[pos].nome, 
        inv->itens[pos].tipo,
        inv->itens[pos].quantidade,
        inv->itens[pos].prioridade);
    printf("\n");
    
}
