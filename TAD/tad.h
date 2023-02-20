#ifndef TAD_H
#define TAD_H
#include <stdio.h>

typedef struct Aluno {
  int numeroInscricao;
  float nota;
  char estado[3];
  char cidade[51];
  char curso[31];
} Aluno;

void printAluno(Aluno aluno);
void fprintAluno(Aluno aluno);
void fscanfAluno(FILE* arquivo, Aluno* aluno);

#endif