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
  int fim;
} Item;

typedef struct BlocoEntrada {
  Item *itens;
  double ultimoInserido;
} BlocoEntrada;

typedef struct Leitura {
  int numeroItens;
  int itensLidos;
} Leitura;

void printAluno(Aluno aluno);
void fprintAluno(Aluno aluno, FILE* arquivo);
void fwriteAluno(Aluno aluno, FILE *arquivo);
void fscanfAluno(FILE* arquivo, Aluno* aluno);
void freadAluno(FILE *arquivo, Aluno *aluno);
void iniciaFita(Fita *fita, FILE *arquivo);
void printaBlocoEntrada(BlocoEntrada bloco);
void iniciaLeitura(Leitura *leitura, int numeroItens);

#endif