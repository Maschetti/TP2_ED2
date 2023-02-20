#include "tad.h"

void printAluno(Aluno aluno) {
  printf("%08d %05.1lf %s %s %s\n", aluno.numeroInscricao, aluno.nota, aluno.estado, aluno.cidade, aluno.curso);
}

void fprintAluno(Aluno aluno) {
  printf("%08d %5.1lf %s %s %s\n", aluno.numeroInscricao, aluno.nota, aluno.estado, aluno.cidade, aluno.curso);
}

void fscanfAluno(FILE* arquivo, Aluno* aluno) {
  fscanf(arquivo, "%d", &aluno->numeroInscricao);
  getc(arquivo);//limpa espaco

  fscanf(arquivo, "%f", &aluno->nota);
  getc(arquivo);//limpa espaco

  fscanf(arquivo, "%s", aluno->estado);
  getc(arquivo);//limpa espaco

  for(int i = 0; i < 50; i++) {
    aluno->cidade[i] = getc(arquivo);
  }
  aluno->cidade[50] = '\0';
  getc(arquivo);//limpa espaco

  for (int i = 0; i < 31; i++) {
    aluno->curso[i] = getc(arquivo);
  }
  aluno->curso[30] = '\0';
  getc(arquivo);//limpa nova linha
}