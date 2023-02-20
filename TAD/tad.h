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

typedef struct Fita {
  FILE *arquivo;
  int numeroBlocos;
} Fita;

typedef struct Item {
  Aluno aluno;
  int marcado;
} Item;

typedef struct BlocoEntrada {
  Item itens[f];
} BlocoEntrada;

void printAluno(Aluno aluno);
void fprintAluno(Aluno aluno);
void fscanfAluno(FILE* arquivo, Aluno* aluno);
void iniciaFita(Fita *fita, FILE *arquivo);
void iniciaItem(Item *item);
void iniciaBlocoEntrada(BlocoEntrada *bloco);

#endif