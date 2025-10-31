#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// --- Variável Global de Pontuação (Nova) ---
int pontuacao = 0;

// --- Estruturas de Dados ---

// Definição da Estrutura da Peça
typedef struct {
    char tipo; // Tipo da peça (ex: 'I', 'O', 'T', 'L', 'J', 'S', 'Z')
    int id;    // ID sequencial da peça
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

// Definição da Pilha de Reserva
#define CAPACIDADE_PILHA 3
typedef struct {
    Peca pecas[CAPACIDADE_PILHA];
    int topo; // Índice do topo da pilha
} PilhaReserva;

// Definição da Pilha de Descarte (Histórico) (Nova Estrutura)
#define CAPACIDADE_DESCARTE 10
typedef struct {
    Peca pecas[CAPACIDADE_DESCARTE];
    int topo;
} PilhaDescarte;


// --- Funções Auxiliares (Comuns) ---

Peca gerarNovaPeca(FilaPecas *fila) {
    Peca novaPeca;
    char tipos[] = {'I', 'O', 'T', 'L', 'J', 'S', 'Z'};
    int num_tipos = sizeof(tipos) / sizeof(tipos[0]);
    novaPeca.tipo = tipos[rand() % num_tipos];
    novaPeca.id = fila->proximo_id++;
    return novaPeca;
}


// --- Funções da FILA (Sem Alterações) ---

void inicializarFila(FilaPecas *fila) {
    fila->frente = 0;
    fila->traseira = -1;
    fila->contador = 0;
    fila->proximo_id = 1;
    srand(time(NULL));
}

int filaCheia(FilaPecas *fila) { return fila->contador == CAPACIDADE_FILA; }
int filaVazia(FilaPecas *fila) { return fila->contador == 0; }

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

// --- Funções da PILHA DE RESERVA (Sem Alterações) ---

void inicializarPilha(PilhaReserva *pilha) { pilha->topo = -1; }
int pilhaCheia(PilhaReserva *pilha) { return pilha->topo == CAPACIDADE_PILHA - 1; }
int pilhaVazia(PilhaReserva *pilha) { return pilha->topo == -1; }

int push(PilhaReserva *pilha, Peca peca) {
    if (pilhaCheia(pilha)) {
        printf(" ERRO: Pilha de reserva cheia! Capacidade máx. é %d.\n", CAPACIDADE_PILHA);
        return 0;
    }
    pilha->topo++;
    pilha->pecas[pilha->topo] = peca;
    return 1;
}

Peca pop(PilhaReserva *pilha) {
    Peca pecaRemovida = {'\0', 0};
    if (pilhaVazia(pilha)) {
        printf(" ERRO: Pilha de reserva vazia! Nenhuma peça para usar.\n");
        return pecaRemovida;
    }
    pecaRemovida = pilha->pecas[pilha->topo];
    pilha->topo--;
    return pecaRemovida;
}

void mostrarPilha(PilhaReserva *pilha) {
    printf("--- Pilha de Reserva (%d/%d) ---\n", pilha->topo + 1, CAPACIDADE_PILHA);
    if (pilhaVazia(pilha)) {
        printf("A pilha está vazia.\n");
        return;
    }
    for (int i = pilha->topo; i >= 0; i--) {
        Peca p = pilha->pecas[i];
        printf("| [%c/%d] ", p.tipo, p.id);
        if (i == pilha->topo) {
            printf(" <- TOPO");
        }
        printf("\n");
    }
    printf("----------------------------------\n");
}


// --- Funções da PILHA DE DESCARTE (Novas Implementações) ---

void inicializarPilhaDescarte(PilhaDescarte *pilha) {
    pilha->topo = -1;
}

// Retorna a peça no topo sem remover (para checagem de bônus)
Peca peekDescarte(PilhaDescarte *pilha) {
    Peca pecaTopo = {'\0', 0};
    if (pilhaVazia(pilha)) {
        return pecaTopo;
    }
    return pilha->pecas[pilha->topo];
}

// Insere um elemento na Pilha de Descarte (com controle de capacidade)
void pushDescarte(PilhaDescarte *pilha, Peca peca) {
    // 1. Verifica se a pilha está cheia
    if (pilha->topo == CAPACIDADE_DESCARTE - 1) {
        printf(" Pilha de Descarte cheia. Descartando peça mais antiga (Base) para abrir espaço.\n");
        
        // Simulação de descarte da base (requer mover todos os elementos para baixo)
        for (int i = 0; i < CAPACIDADE_DESCARTE - 1; i++) {
            pilha->pecas[i] = pilha->pecas[i+1];
        }
        // O topo permanece no mesmo índice, mas agora aponta para o penúltimo elemento anterior
        // (O espaço no final foi liberado)
        pilha->topo = CAPACIDADE_DESCARTE - 2; // Aponta para o novo último elemento (ou -1 se for 0)
    }

    // 2. Insere a nova peça no topo
    pilha->topo++;
    pilha->pecas[pilha->topo] = peca;
}

void mostrarPilhaDescarte(PilhaDescarte *pilha) {
    printf("--- Pilha de Descarte (Histórico) (%d/%d) ---\n", pilha->topo + 1, CAPACIDADE_DESCARTE);
    if (pilhaVazia((PilhaReserva*)pilha)) { // Reutilizando a verificação vazia
        printf("O histórico está vazio.\n");
        return;
    }
    
    // Exibe do topo (mais recente) para a base (mais antiga)
    for (int i = pilha->topo; i >= 0; i--) {
        Peca p = pilha->pecas[i];
        printf("| [%c/%d] ", p.tipo, p.id);
        if (i == pilha->topo) {
            printf(" <- TOPO (Mais Recente)");
        }
        printf("\n");
    }
    printf("==================================================\n");
    printf("===================================================\n");
}

// Processa uma peça jogada (Usado nas opções 1 e 3)
void processarPecaJogada(Peca pecaJogada, PilhaDescarte *descarte) {
    // 1. Checa a Condição de Bônus (ANTES de inserir a nova peça)
    Peca topoDescarte = peekDescarte(descarte);
    
    if (topoDescarte.id != 0 && pecaJogada.tipo == topoDescarte.tipo) {
        pontuacao += 100;
        printf("BÔNUS! Tipo '%c' jogado é igual ao topo do histórico! (+100 pontos) \n");
    }

    // 2. Adiciona a peça jogada ao Descarte
    pushDescarte(descarte, pecaJogada);
}


// --- Função Principal e Menu (Nível Mestre) ---

int main() {
    FilaPecas fila;
    PilhaReserva pilha;
    PilhaDescarte descarte; // Nova Pilha
    
    inicializarFila(&fila);
    inicializarPilha(&pilha);
    inicializarPilhaDescarte(&descarte); // Inicializa Descarte
    
    int opcao;

    printf(" Tetris Stack Challenge - Nível Mestre \n");
    
    // Preencher a fila inicialmente (com 5 peças)
    while (!filaCheia(&fila)) {
        enqueue(&fila, gerarNovaPeca(&fila));
    }

    // Exibir o estado inicial
    printf("Pontuação Atual: %d\n", pontuacao);
    mostrarFila(&fila);
    mostrarPilha(&pilha);
    mostrarPilhaDescarte(&descarte); // Mostrar o descarte

    do {
        printf("\n--- JOGO ---\n");
        printf("Pontuação Atual: **%d**\n", pontuacao);
        printf("Escolha uma opção:\n");
        printf(" 1 -  Jogar peça da **Fila** (e repor)\n");
        printf(" 2 -  Enviar peça da **Fila** para a **Reserva**\n");
        printf(" 3 -  Usar peça da **Reserva**\n");
        printf(" 0 -  Sair\n");
        printf("Opção: ");
        scanf("%d", &opcao);
        
        // Variável auxiliar para a peça jogada
        Peca pecaJogada = {'\0', 0};

        switch (opcao) {
            case 1: { // Jogar Peça (Da Fila)
                pecaJogada = dequeue(&fila);
                if (pecaJogada.id != 0) {
                    printf(" Peça jogada (da Fila): Tipo **%c**, ID **%d**.\n", pecaJogada.tipo, pecaJogada.id);
                    
                    // Processa descarte e bônus
                    processarPecaJogada(pecaJogada, &descarte);
                    
                    // Manter a fila sempre cheia: Repõe
                    enqueue(&fila, gerarNovaPeca(&fila));
                    printf(" Nova peça adicionada à Fila.\n");
                    printf("------------------------------\n");
                }
                break;
            }
            case 2: { // Enviar Peça da Fila para a Reserva (Pilha)
                if (pilhaCheia(&pilha)) break;
                
                Peca pecaReserva = dequeue(&fila);
                if (pecaReserva.id != 0) {
                    if (push(&pilha, pecaReserva)) {
                        printf(" Peça enviada da Fila para a Reserva: Tipo **%c**, ID **%d**.\n", pecaReserva.tipo, pecaReserva.id);
                        // Manter a fila sempre cheia: Repõe
                        enqueue(&fila, gerarNovaPeca(&fila));
                        printf(" Nova peça adicionada à Fila para repor.\n");
                        printf("-------------------------------------------\n");
                    }
                }
                break;
            }
            case 3: { // Usar Peça da Reserva (Da Pilha)
                pecaJogada = pop(&pilha);
                if (pecaJogada.id != 0) {
                    printf(" Peça usada (da Reserva): Tipo **%c**, ID **%d**.\n", pecaJogada.tipo, pecaJogada.id);
                    
                    // Processa descarte e bônus
                    processarPecaJogada(pecaJogada, &descarte);
                }
                break;
            }
            case 0:
                printf("Saindo do simulador. Pontuação final: **%d**.\n", pontuacao);
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
        
        // Exibir o estado após a ação
        if (opcao != 0) {
            mostrarFila(&fila);
            mostrarPilha(&pilha);
            mostrarPilhaDescarte(&descarte);
        }

    } while (opcao != 0);

    return 0;
}