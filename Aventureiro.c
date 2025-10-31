#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// --- Estruturas de Dados ---

// Definição da Estrutura da Peça
typedef struct {
    char tipo; // Tipo da peça (ex: 'I', 'O', 'T', 'L', 'J', 'S', 'Z')
    int id;   // ID sequencial da peça
} Peca;

// Definição da Fila Circular de Peças Futuras
#define CAPACIDADE_FILA 5 
typedef struct {
    Peca pecas[CAPACIDADE_FILA];
    int frente;
    int traseira;
    int contador;
    int proximo_id; // Próximo ID a ser atribuído
} FilaPecas;

// Definição da Pilha de Reserva (Nova Estrutura)
#define CAPACIDADE_PILHA 3
typedef struct {
    Peca pecas[CAPACIDADE_PILHA];
    int topo; // Índice do topo da pilha
} PilhaReserva;


// --- Funções Auxiliares (Comuns) ---

// Gera uma nova peça com tipo aleatório e ID sequencial
Peca gerarNovaPeca(FilaPecas *fila) {
    Peca novaPeca;
    char tipos[] = {'I', 'O', 'T', 'L', 'J', 'S', 'Z'};
    int num_tipos = sizeof(tipos) / sizeof(tipos[0]);
    novaPeca.tipo = tipos[rand() % num_tipos];
    novaPeca.id = fila->proximo_id++;
    return novaPeca;
}


// --- Funções da FILA (Reutilizadas e Adaptadas) ---

void inicializarFila(FilaPecas *fila) {
    fila->frente = 0;
    fila->traseira = -1;
    fila->contador = 0;
    fila->proximo_id = 1;
    srand(time(NULL));
}

int filaCheia(FilaPecas *fila) {
    return fila->contador == CAPACIDADE_FILA;
}

int filaVazia(FilaPecas *fila) {
    return fila->contador == 0;
}

int enqueue(FilaPecas *fila, Peca peca) {
    if (filaCheia(fila)) return 0;

    fila->traseira = (fila->traseira + 1) % CAPACIDADE_FILA;
    fila->pecas[fila->traseira] = peca;
    fila->contador++;
    return 1;
}

Peca dequeue(FilaPecas *fila) {
    Peca pecaRemovida = {'\0', 0};
    if (filaVazia(fila)) return pecaRemovida;

    pecaRemovida = fila->pecas[fila->frente];
    fila->frente = (fila->frente + 1) % CAPACIDADE_FILA;
    fila->contador--;
    return pecaRemovida;
}

void mostrarFila(FilaPecas *fila) {
    printf("\n--- Fila de Peças Futuras (%d/%d) ---\n", fila->contador, CAPACIDADE_FILA);
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
}

// --- Funções da PILHA (Novas Implementações) ---

// Inicializa a Pilha
void inicializarPilha(PilhaReserva *pilha) {
    pilha->topo = -1; // -1 indica pilha vazia
}

// Verifica se a Pilha está Cheia
int pilhaCheia(PilhaReserva *pilha) {
    return pilha->topo == CAPACIDADE_PILHA - 1;
}

// Verifica se a Pilha está Vazia
int pilhaVazia(PilhaReserva *pilha) {
    return pilha->topo == -1;
}

// Insere um elemento na Pilha (push)
int push(PilhaReserva *pilha, Peca peca) {
    if (pilhaCheia(pilha)) {
        printf(" ERRO: Pilha de reserva cheia! Capacidade máx. é %d.\n", CAPACIDADE_PILHA);
        return 0; // Falha
    }
    pilha->topo++;
    pilha->pecas[pilha->topo] = peca;
    return 1; // Sucesso
}

// Remove um elemento da Pilha (pop)
Peca pop(PilhaReserva *pilha) {
    Peca pecaRemovida = {'\0', 0}; // Peça nula para erro
    if (pilhaVazia(pilha)) {
        printf(" ERRO: Pilha de reserva vazia! Nenhuma peça para usar.\n");
        return pecaRemovida;
    }
    pecaRemovida = pilha->pecas[pilha->topo];
    pilha->topo--;
    return pecaRemovida;
}

// Exibe o conteúdo atual da Pilha
void mostrarPilha(PilhaReserva *pilha) {
    printf("--- Pilha de Reserva (%d/%d) ---\n", pilha->topo + 1, CAPACIDADE_PILHA);
    if (pilhaVazia(pilha)) {
        printf("A pilha está vazia.\n");
        return;
    }
    
    // Exibe do topo para a base
    for (int i = pilha->topo; i >= 0; i--) {
        Peca p = pilha->pecas[i];
        printf("| [%c/%d] ", p.tipo, p.id);
        if (i == pilha->topo) {
            printf(" <- TOPO (Próx. a sair)");
        }
        printf("\n");
    }
    printf("----------------------------------\n");
}


// --- Função Principal e Menu (Integrando Fila e Pilha) ---

int main() {
    FilaPecas fila;
    PilhaReserva pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);
    int opcao;

    printf(" Tetris Stack Challenge - Nível Aventureiro \n");
    
    // Preencher a fila inicialmente (com 5 peças)
    while (!filaCheia(&fila)) {
        enqueue(&fila, gerarNovaPeca(&fila));
    }

    // Exibir o estado inicial
    mostrarFila(&fila);
    mostrarPilha(&pilha);

    do {
        printf("\n");
        printf("Escolha uma opção:\n");
        printf(" 1 -  Jogar peça (remover da **Fila** e repor com nova)\n");
        printf(" 2 -  Enviar peça da **Fila** para a **Reserva** (Pilha)\n");
        printf(" 3 -  Usar peça da **Reserva** (remover do topo da Pilha)\n");
        printf(" 0 -  Sair\n");
        printf("Opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: { // Jogar Peça (Da Fila)
                Peca pecaAtual = dequeue(&fila);
                if (pecaAtual.id != 0) {
                    printf(" Peça jogada (da Fila): Tipo **%c**, ID **%d**.\n", pecaAtual.tipo, pecaAtual.id);
                    // Manter a fila sempre cheia: Insere uma nova peça no final
                    enqueue(&fila, gerarNovaPeca(&fila));
                    printf(" Nova peça adicionada à Fila.\n");
                }
                break;
            }
            case 2: { // Enviar Peça da Fila para a Reserva (Pilha)
                if (pilhaCheia(&pilha)) {
                    // Mensagem de erro já na função push
                    break;
                }
                Peca pecaReserva = dequeue(&fila);
                if (pecaReserva.id != 0) {
                    if (push(&pilha, pecaReserva)) {
                        printf(" Peça enviada da Fila para a Reserva: Tipo **%c**, ID **%d**.\n", pecaReserva.tipo, pecaReserva.id);
                        // Manter a fila sempre cheia: Insere uma nova peça no final
                        enqueue(&fila, gerarNovaPeca(&fila));
                        printf(" Nova peça adicionada à Fila para repor.\n");
                    }
                }
                break;
            }
            case 3: { // Usar Peça da Reserva (Da Pilha)
                Peca pecaReserva = pop(&pilha);
                if (pecaReserva.id != 0) {
                    printf(" Peça usada (da Reserva): Tipo **%c**, ID **%d**.\n", pecaReserva.tipo, pecaReserva.id);
                    // Atenção: O requisito NÃO pede para repor a PILHA, apenas para manter a FILA cheia.
                }
                break;
            }
            case 0:
                printf("Saindo do simulador. Até mais!\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
        
        // Exibir o estado após a ação
        if (opcao != 0) {
            mostrarFila(&fila);
            mostrarPilha(&pilha);
        }

    } while (opcao != 0);

    return 0;
}