#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 3

typedef struct {
    char nome[30];
    int vitorias;
} Jogador;

void exibirTabuleiro(char **tabuleiro);
void inicializarTabuleiro(char **tabuleiro);
int verificarVencedor(char **tabuleiro);
void salvarRanking(Jogador jogador);
void exibirRanking();
void jogar(char **tabuleiro, int modo);
void exibirCreditos();

int main() {
    char **tabuleiro;
    int opcao;

    tabuleiro = (char **)malloc(SIZE * sizeof(char *));
    for (int i = 0; i < SIZE; i++) {
        tabuleiro[i] = (char *)malloc(SIZE * sizeof(char));
    }

    do {
        printf("\n === Jogo da Velha === \n");
        printf("1. Jogar \n");
        printf("2. Ver Ranking \n");
        printf("3. Creditos \n");
        printf("4. Sair \n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
        case 1:
            printf("\nEscolha o modo de jogo:\n");
            printf("1. Jogador vs Computador\n");
            printf("2. Jogador vs Jogador\n");
            int modo;
            scanf("%d", &modo);
            if (modo == 1 || modo == 2) {
                jogar(tabuleiro, modo);
            } else {
                printf("Modo invalido! Retornando ao menu.\n");
            }
            break;
        case 2:
            exibirRanking();
            break;
        case 3:
            exibirCreditos();
            break;
        case 4:
            printf("Saindo do jogo...\n");
            break;
        default:
            printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 4);

    for (int i = 0; i < SIZE; i++) {
        free(tabuleiro[i]);
    }
    free(tabuleiro);

    return 0;
}

void exibirTabuleiro(char **tabuleiro) {
    printf("\nTabuleiro:\n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf(" %c ", tabuleiro[i][j]);
            if (j < SIZE - 1) printf("|");
        }
        printf("\n");
        if (i < SIZE - 1) printf("---|---|---\n");
    }
}

void inicializarTabuleiro(char **tabuleiro) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            tabuleiro[i][j] = ' ';
        }
    }
}

int verificarVencedor(char **tabuleiro) {
    for (int i = 0; i < SIZE; i++) {
      
        if ((tabuleiro[i][0] == tabuleiro[i][1] && tabuleiro[i][1] == tabuleiro[i][2] && tabuleiro[i][0] != ' ') ||
            (tabuleiro[0][i] == tabuleiro[1][i] && tabuleiro[1][i] == tabuleiro[2][i] && tabuleiro[0][i] != ' '))
            return tabuleiro[i][i] == 'X' ? 1 : 2;
    }

    if ((tabuleiro[0][0] == tabuleiro[1][1] && tabuleiro[1][1] == tabuleiro[2][2] && tabuleiro[0][0] != ' ') ||
        (tabuleiro[0][2] == tabuleiro[1][1] && tabuleiro[1][1] == tabuleiro[2][0] && tabuleiro[1][1] != ' '))
        return tabuleiro[1][1] == 'X' ? 1 : 2;

    return 0;
}

void salvarRanking(Jogador jogador) {
    FILE *arquivo = fopen("ranking.txt", "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de ranking.\n");
        return;
    }
    fprintf(arquivo, "%s %d\n", jogador.nome, jogador.vitorias);
    fclose(arquivo);
}

void exibirRanking() {
    FILE *arquivo = fopen("ranking.txt", "r");
    if (arquivo == NULL) {
        printf("Ainda nao ha um ranking registrado. \n");
        return;
    }

    char nome[30];
    int vitorias;

    printf("\n=== Ranking ===\n");
    while (fscanf(arquivo, "%s %d", nome, &vitorias) != EOF) {
        printf("%s - %d vitoria(s)\n", nome, vitorias);
    }

    fclose(arquivo);
}

void jogar(char **tabuleiro, int modo) {
    Jogador jogador1, jogador2;
    int linha, coluna, turno = 0, vencedor;

    printf("Digite o nome do Jogador 1: ");
    scanf("%s", jogador1.nome);
    jogador1.vitorias = 0;

    if (modo == 2) {
        printf("Digite o nome do Jogador 2: ");
        scanf("%s", jogador2.nome);
    } else {
        strcpy(jogador2.nome, "Computador");
    }

    inicializarTabuleiro(tabuleiro);

    while (turno < SIZE * SIZE) {
        exibirTabuleiro(tabuleiro);

        if (turno % 2 == 0) {
            printf("%s, sua vez! Informe a linha e a coluna (0-2): ", jogador1.nome);
        } else {
            printf("%s, sua vez! Informe a linha e a coluna (0-2): ", jogador2.nome);
        }

        if (modo == 1 && turno % 2 != 0) {
            do {
                linha = rand() % SIZE;
                coluna = rand() % SIZE;
            } while (tabuleiro[linha][coluna] != ' ');
            printf("Computador jogou na posicao (%d, %d)\n", linha, coluna);
        } else {
            scanf("%d %d", &linha, &coluna);

            if (linha < 0 || linha >= SIZE || coluna < 0 || coluna >= SIZE || tabuleiro[linha][coluna] != ' ') {
                printf("Jogada invalida! Tente novamente.\n");
                continue;
            }
        }

        tabuleiro[linha][coluna] = (turno % 2 == 0) ? 'X' : 'O';

        vencedor = verificarVencedor(tabuleiro);
        if (vencedor) break;

        turno++;
    }

    exibirTabuleiro(tabuleiro);

    if (vencedor == 1) {
        printf("Parabens, %s! Voce venceu! \n", jogador1.nome);
        jogador1.vitorias++;
        salvarRanking(jogador1);
    } else if (vencedor == 2) {
        printf("Parabens, %s! Voce venceu! \n", jogador2.nome);
        if (modo == 2) salvarRanking(jogador2);
    } else {
        printf("Empate! \n");
    }
}

void exibirCreditos() {
    printf("\n === Creditos === \n");
    printf("Jogo desenvolvido por: [Vitor Ferraz, Calebe Paiva, Diogenes Italiano, Jose Felipe bandeira e Miguel Araujo] \n");
    printf("Versao: 2.0 \n");
    printf("Aproveite o jogo! \n"); 
}
