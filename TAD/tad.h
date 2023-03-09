#ifndef TAD_H
#define TAD_H
#include <stdio.h>

#define f 10

typedef struct Aluno {
  int numeroInscricao;
  float nota;
  char estado[3];
  char cidade[51];
  char curso[31];
} Aluno;

typedef struct Item {
  Aluno aluno;
  int marcado;
  int fim;
} Item;

typedef struct BlocoEntrada {
  Item *itens;
  double ultimoInserido;
} BlocoEntrada;

typedef struct Intercalador {
  Aluno aluno;
  int fimBloco;
  int fimArquivo;
} Intercalador;

typedef struct Area {
  Aluno alunos[f];
  int quantidade;
} Area;

void printAluno(Aluno aluno);
void fprintAluno(Aluno aluno, FILE* arquivo);
void fscanfAluno(FILE* arquivo, Aluno* aluno);
void printaBlocoEntrada(BlocoEntrada bloco);
int comparaIntercalador(Intercalador x, Intercalador y);

#endif