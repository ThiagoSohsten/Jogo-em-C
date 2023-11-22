#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include "snake.h"

#define COLUNAS 40
#define LINHAS 20

int *x;
int *y;
int sair = 0;
int macaX = 0, macaY = 0;
int cauda = 0, cabeca = 0;
int fimDoJogo = 0;
int direcaox = 1, direcaoy = 0;
struct termios configAntiga;
char matriz[LINHAS][COLUNAS];

void configurarJogo() {
  printf("\e[?25l");
  struct termios configNova;
  tcgetattr(STDIN_FILENO, &configAntiga);
  configNova = configAntiga;
  configNova.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &configNova);

  for (int i = 0; i < LINHAS; i++) {
    for (int j = 0; j < COLUNAS; j++) {
      matriz[i][j] = ' ';
    }
  }
}

void iniciarJogo() {
  x = (int *)malloc(1000 * sizeof(int));
  y = (int *)malloc(1000 * sizeof(int));
  if (x == NULL || y == NULL) {
    perror("Erro ao alocar memória");
    return;
  }

  while (!sair) {
    for (int i = 0; i < LINHAS; i++) {
      for (int j = 0; j < COLUNAS; j++) {
        matriz[i][j] = ' ';
      }
    }
    
    for (int i = cauda; i != cabeca; i = (i + 1) % 1000) {
      matriz[y[i]][x[i]] = 'o';
    }
    matriz[y[cabeca]][x[cabeca]] = 'O';
  if (macaX >= 0) {
  matriz[macaY][macaX] = 'H';
}

    printf("┌");
    for (int i = 0; i < COLUNAS; i++)
      printf("─");
    printf("┐\n");

    for (int j = 0; j < LINHAS; j++) {
      printf("│");
      for (int i = 0; i < COLUNAS; i++)
        printf("%c", matriz[j][i]);
      printf("│\n");
    }

    printf("└");
    for (int i = 0; i < COLUNAS; i++)
      printf("─");
    printf("┘\n");

    printf("\e[%iA", LINHAS + 2);

    int cabeca = 0, cauda = 0;
    x[cabeca] = COLUNAS / 2;
    y[cabeca] = LINHAS / 2;
    int fimDoJogo = 0;
    int direcaox = 1, direcaoy = 0;
    int macaX = -1, macaY;

    while (!sair && !fimDoJogo) {
      if (macaX < 0) {
        macaX = rand() % COLUNAS;
        macaY = rand() % LINHAS;

        for (int i = cauda; i != cabeca; i = (i + 1) % 1000)
          if (x[i] == macaX && y[i] == macaY)
            macaX = -1;

        if (macaX >= 0) {
          printf("\e[%iB\e[%iC❤", macaY + 1, macaX + 1);
          printf("\e[%iF", macaY + 1);
        }
      }

      printf("\e[%iB\e[%iC·", y[cauda] + 1, x[cauda] + 1);
      printf("\e[%iF", y[cauda] + 1);

      if (x[cabeca] == macaX && y[cabeca] == macaY) {
        macaX = -1;
        printf("\a");
      } else
        cauda = (cauda + 1) % 1000;

      int novaCabeca = (cabeca + 1) % 1000;
      x[novaCabeca] = (x[cabeca] + direcaox + COLUNAS) % COLUNAS;
      y[novaCabeca] = (y[cabeca] + direcaoy + LINHAS) % LINHAS;
      cabeca = novaCabeca;

      for (int i = cauda; i != cabeca; i = (i + 1) % 1000)
        if (x[i] == x[cabeca] && y[i] == y[cabeca])
          fimDoJogo = 1;

      printf("\e[%iB\e[%iC▓", y[cabeca] + 1, x[cabeca] + 1);
      printf("\e[%iF", y[cabeca] + 1);
      fflush(stdout);

      usleep(5 * 1000000 / 60);

      struct timeval tempo;
      fd_set descritores;
      tempo.tv_sec = 0;
      tempo.tv_usec = 0;

      FD_ZERO(&descritores);
      FD_SET(STDIN_FILENO, &descritores);
      select(STDIN_FILENO + 1, &descritores, NULL, NULL, &tempo);
      if (FD_ISSET(STDIN_FILENO, &descritores)) {
        int tecla = getchar();
        if (tecla == 27 || tecla == 'q') {
          sair = 1;
        } else if (tecla == 'w' && direcaoy != 1) {
          direcaox = 0;
          direcaoy = -1;
        } else if (tecla == 'a' && direcaox != 1) {
          direcaox = -1;
          direcaoy = 0;
        } else if (tecla == 's' && direcaoy != -1) {
          direcaox = 0;
          direcaoy = 1;
        } else if (tecla == 'd' && direcaox != -1) {
          direcaox = 1;
          direcaoy = 0;
        }
      }
    }

    if (!sair) {
      printf("\e[%iB\e[%iC Fim do Jogo! ", LINHAS / 2, COLUNAS / 2 - 5);
      printf("\e[%iF", LINHAS / 2);
      fflush(stdout);
      getchar();
    }
  }

  printf("\e[?25h");

  tcsetattr(STDIN_FILENO, TCSANOW, &configAntiga);
}

void encerrarJogo() {
  printf("\e[?25h");

  tcsetattr(STDIN_FILENO, TCSANOW, &configAntiga);
  free(x);
  free(y);
}
