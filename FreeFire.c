#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <locale.h>

// Código da Ilha ? Edição Free Fire
// Nível: Mestre
// Este programa simula o gerenciamento avançado de uma mochila com componentes coletados durante a fuga de uma ilha.

// Definições fixa do tamanho durante a execução do programa nome e tipo do item
#define TAM_NOME 30
#define TAM_TIPO 20

// Define uma contante para o número máximo de itens que podemos ter
#define TAM_MAX 10 

// Struct Item:
// Representa um componente com nome, tipo, quantidade.
// Agora vamos criar nossa estrutura Item com o comando struct 
// Representando as informações de 1 item
typedef struct {
    char nome[TAM_NOME];
    char tipo[TAM_TIPO];
    int quantidade; 
} Item;

// Estrutura que representa o inventário completo, com uma variavel que representa o contador de itens no Inventario
typedef struct {
    Item itens[TAM_MAX];
    int numeroItens;
} Inventario;

// Função para limpar o buffer de entrada
void liberarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Função para remover a quebra de linha (\n) lida pelo fgets
void limpa_quebra_linha(char *string) {
    string[strcspn(string, "\n")] = '\0';
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

int main() {
    setlocale(LC_ALL,"Portuguese_Brazil");
    
    // Criação da variável da struct Inventario
    Inventario inventario;

    // Inicializador do contador de itens começando com zero
    inventario.numeroItens = 0;

    // Variavel inteiro para representar a escolha do usuario
    // Será usada na estrutura switch
    int opcao;

    do { 
        printf("\n");      
        printf("========================================="); 
        printf("\n   MOCHILA DE SOBREVIVÊNCIA - CODIGO DA ILHA \n");
        printf("=========================================\n"); 

        // Representação da quantidade de itens na mochila
        // Opções de escolha do usuario representando por número inteiro que pode digitar
        printf("Itens na Mochila: %d/10\n",inventario.numeroItens);  
        printf("\n");        
        printf("1. Adicionar um Item (Loot) \n");        
        printf("2. Remover Item\n");        
        printf("3. Listar Itens na Mochila \n"); 
        printf("4. Buscar Item por Nome \n");        
        printf("0. Sair\n");
        printf("-----------------------------\n");
        printf("Escolha uma opção: ");

        // Loop para garantir uma entrada válida para escolha
        // Se o usuario digitar algoi diferente de um numero inteiro
        // Ou digitar um numero maior que o numero de opções (3)
        // Ou digitar um numero menor que o numero de opções (0)
        // O programa notificará o erro
        while (scanf("%d", &opcao) != 1 || opcao > 4 || opcao < 0 ) {
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
                listarItens(&inventario);
                printf("Pressione ENTER para continuar... ");
                // Comando que espera o usuario para prosseguir a execução do programa com o ENTER
                getchar();
                break;
            case 2:
                removeritem(&inventario);
                break;
            case 3:
                listarItens(&inventario);
                break;
            case 4:
                printf(" --- Buscar Item na Mochila --- \n");
                buscaritem(&inventario);
                printf("Pressione ENTER para continuar... ");
                // Comando que espera o usuario para prosseguir a execução do programa com o ENTER
                getchar();
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
    printf("Nome do item: ");

    // Comando fgets para extrair a entrada do usuario
    fgets(novo.nome, TAM_NOME, stdin);

    // O fgets insere uma linha após a execução, logo, esta função excluirá esta linha
    limpa_quebra_linha(novo.nome);

    printf("Tipo do item (arma, munição, cura, etc.): ");
    fgets(novo.tipo, TAM_TIPO, stdin);
    limpa_quebra_linha(novo.tipo);

    printf("Quantidade: ");
    scanf("%d", &novo.quantidade);
    liberarBuffer();

    // Atribui o novo item ao indice de itens
    inv->itens[inv->numeroItens] = novo;

    // Atualiza a quantidade de itens dentro do inventario
    inv->numeroItens++;
    printf("\n");
    printf("Item \"%s\" adicionado com sucesso!\n", novo.nome);
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
    printf("Digite o nome do item a remover: ");
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
        printf("Erro: item \"%s\" nao encontrado.\n", nomeRemover);
        return;
    }

    // Estrutura que percorre os itens atualizando suas posições
    // Porque caso um item seja removido, o indice das variaveis itens do inventario tem que ser atualizado
    for (int i = pos; i < inv->numeroItens - 1; i++) {
        inv->itens[i] = inv->itens[i + 1];
    }

    // Atualização da quantidade de intens dentro de inventario
    inv->numeroItens--;
    printf("Item \"%s\" removido com sucesso.\n", nomeRemover);
}

// listarItens():
// Exibe uma tabela formatada com todos os componentes presentes na mochila.
void listarItens(const Inventario *inv) {

    // Tratamento de erros para o caso de o inventario não ter itens para listar
    if (inv->numeroItens == 0) {
        printf("O inventario esta vazio.\n");
        return;
    }

    printf("\n--- ITENS NA MOCHILA (%d/%d) ---\n", inv->numeroItens, TAM_MAX);
    printf("-------------------------------------------------------------\n");
    printf("NOME                       | TIPO           | QUANTIDADE     \n");
    printf("-------------------------------------------------------------\n");

    // A estrutura for percorre o itens no inventario para listá-los
    // Após a porcentagem temos o numero de espaços que serão alocados com o conteudo das caracteristicas de cada itens
    for (int i = 0; i < inv->numeroItens; i++) {
        printf("%-26s | %-14s | %3d\n",
               inv->itens[i].nome,
               inv->itens[i].tipo,
               inv->itens[i].quantidade);
    }
    printf("-------------------------------------------------------------\n");
}

void buscaritem(Inventario *inv) {
    // Tratamento de erros para o caso de o inventario não ter itens para buscar
    if (inv->numeroItens == 0) {
        printf("Inventario vazio!\n");
        return;
    }

    // variavel com a entrada do usuário do item que queremos buscar
    char nomeItem[TAM_NOME];
    printf("Digite o nome do item que deseja buscar: ");
    fgets(nomeItem, TAM_NOME, stdin);
    limpa_quebra_linha(nomeItem);

    // Variável de inicialização da posição, quando encontrar o item esta variavel terá a posição do item desejado
    int pos = -1;

    // O for irá percorrer sequencialmente todo o vetor
    for (int i = 0; i < inv->numeroItens;i++){

        // COmando strcmp vai comparar o indice vetor com o nome do item desejado
        // Encontrando, mudará a variavel pos, atribuindo o indice do item desejado
        if (strcmp(inv->itens[i].nome, nomeItem) == 0) {
            pos = i;
            break;
        }
    }

    // Caso a função não encontre o item com o nome igual ao do usuario
    // A pos continuará sendo a que inicializou, logo, o item não foi encontrado
    if (pos == -1) {
        printf("Resultado: Item \"%s\" NÃO foi encontrado na mochila.\n", nomeItem);
        return;
    }

    // A partir do momento que o item for encontrado
    // Este printf imprimir o nome, tipo e quantidade do item desejado
    printf("\n--- Item Encontrado! --- \n");
    printf("Nome: \"%s\".\n", inv->itens[pos].nome);
    printf("Tipo: \"%s\".\n", inv->itens[pos].tipo);
    printf("Quantidade: \"%d\".\n", inv->itens[pos].quantidade);
    printf("-------------------------\n");
    printf("\n");
}