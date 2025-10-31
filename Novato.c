#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 1. Definição da Estrutura da Peça (struct Peca)
typedef struct {
    char tipo; // Tipo da peça (ex: 'I', 'O', 'T', 'L', 'J', 'S', 'Z')
    int id;   // identificador sequencial da peça
} Peca;

// 2. Extrutura da Fila Circular de Peças Futuras
#define CAPACIDADE_FILA 5 // Capacidade máxima da fila
typedef struct {
    Peca pecas[CAPACIDADE_FILA];
    int frente; // Índice da frente da fila
    int traseira; // Índice da traseira da fila
    int contador; // Número atual de elementos na fila
    int proximo_id; // Próximo ID a ser atribuído a uma nova peça
} FilaPecas;

// --- Implementação das Funções da Fila ---

// Inicializa a Fila
void inicializarFila(FilaPecas *fila) {
    fila->frente = 0;
    fila->traseira = -1; // -1 indica que a fila está vazia (convenção)
    fila->contador = 0;
    fila->proximo_id = 1; // Começa o ID das peças em 1
    // Inicializa o gerador de números aleatórios
    srand(time(NULL));
}

// Verifica se a Fila está Cheia
int filaCheia(FilaPecas *fila) {
    return fila->contador == CAPACIDADE_FILA;
}

// Verifica se a Fila está Vazia
int filaVazia(FilaPecas *fila) {
    return fila->contador == 0;
}

// Gera uma nova peça com tipo aleatório e ID sequencial
Peca gerarNovaPeca(FilaPecas *fila) {
    Peca novaPeca;
    
    // Tipos de peças de Tetris
    char tipos[] = {'I', 'O', 'T', 'L', 'J', 'S', 'Z'};
    int num_tipos = sizeof(tipos) / sizeof(tipos[0]);

    // Seleciona um tipo aleatório
    novaPeca.tipo = tipos[rand() % num_tipos];
    
    // Atribui o ID sequencial e incrementa o contador de ID
    novaPeca.id = fila->proximo_id++;

    return novaPeca;
}

// Insere um elemento na Fila (enqueue)
int enqueue(FilaPecas *fila, Peca peca) {
    if (filaCheia(fila)) {
        printf(" ERRO: Fila de peças futura cheia!\n");
        return 0; // Falha na inserção
    }

    // Calcula o próximo índice da traseira de forma circular
    fila->traseira = (fila->traseira + 1) % CAPACIDADE_FILA;
    fila->pecas[fila->traseira] = peca;
    fila->contador++;
    
    return 1; // Sucesso na inserção
}

// Remove um elemento da Fila (dequeue)
Peca dequeue(FilaPecas *fila) {
    Peca pecaRemovida = {'\0', 0}; // Peça nula para erro
    
    if (filaVazia(fila)) {
        printf(" ERRO: Fila de peças futura vazia!\n");
        return pecaRemovida; // Retorna peça nula
    }

    pecaRemovida = fila->pecas[fila->frente];
    
    // Calcula o próximo índice da frente de forma circular
    fila->frente = (fila->frente + 1) % CAPACIDADE_FILA;
    fila->contador--;
    
    return pecaRemovida;
}

// Exibe o conteúdo atual da Fila
void mostrarFila(FilaPecas *fila) {
    printf("\n--- Fila de Peças Futuras (Capacidade: %d/%d) ---\n", fila->contador, CAPACIDADE_FILA);

    if (filaVazia(fila)) {
        printf("A fila está vazia.\n");
        return;
    }

    printf("Frente -> ");
    int i = fila->frente;
    int elementos = fila->contador;

    for (int count = 0; count < elementos; count++) {
        Peca p = fila->pecas[i];
        printf("| [%c/%d] ", p.tipo, p.id);
        i = (i + 1) % CAPACIDADE_FILA;
    }
    printf(" <- Traseira\n");
    printf("----------------------------------------------------\n");
}


// --- Função Principal e Menu ---

int main() {
    FilaPecas fila;
    inicializarFila(&fila);
    int opcao;

    printf(" Bem-vindo ao Tetris Stack Challenge - Nível Novato \n");
    printf("Inicializando Fila de Peças...\n");
    
    // 4. Preencher a fila inicialmente (com 5 peças)
    while (!filaCheia(&fila)) {
        enqueue(&fila, gerarNovaPeca(&fila));
    }

    // Exibir o estado inicial
    mostrarFila(&fila);

    do {
        printf("\n");
        printf("Escolha uma opção:\n");
        printf(" 1 - Jogar peça (remover da frente e adicionar nova no final)\n");
        printf(" 0 -  Sair\n");
        printf("Opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                if (filaVazia(&fila)) {
                    printf("A fila está vazia! Nenhuma peça para jogar.\n");
                } else {
                    // 1. Remover a peça da frente (Jogar peça)
                    Peca pecaAtual = dequeue(&fila);
                    printf("  Peça jogada: Tipo **%c**, ID **%d**.\n", pecaAtual.tipo, pecaAtual.id);

                    // 2. Gerar uma nova peça e Inserir no final
                    Peca novaPeca = gerarNovaPeca(&fila);
                    if (enqueue(&fila, novaPeca)) {
                        printf(" Nova peça adicionada à fila: Tipo **%c**, ID **%d**.\n", novaPeca.tipo, novaPeca.id);
                    }
                }
                // 3. Exibir o estado atual da fila
                mostrarFila(&fila);
                break;
            }
            case 0:
                printf("Saindo do simulador. Até mais!\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}