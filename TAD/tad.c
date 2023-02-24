#include "tad.h"

void printAluno(Aluno aluno) {
  printf("%08d %05.1lf %s %s %s\n", aluno.numeroInscricao, aluno.nota, aluno.estado, aluno.cidade, aluno.curso);
}

void fprintAluno(Aluno aluno, FILE* arquivo) {
  fprintf(arquivo, "%08d %05.1lf %s %s %s\n", aluno.numeroInscricao, aluno.nota, aluno.estado, aluno.cidade, aluno.curso);
}

void fwriteAluno(Aluno aluno, FILE *arquivo) {
  fwrite(&aluno, sizeof(Aluno), 1, arquivo);
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

void freadAluno(FILE *arquivo, Aluno *aluno) {
  fread(aluno, sizeof(Aluno), 1, arquivo);
}

void iniciaFita(Fita *fita, FILE *arquivo) {
  fita->arquivo = arquivo;
  fita->numeroBlocos = 0;
}

void zeraItem(Item *item) {
  item->marcado = 0;
}

void zeraBlocoEntrada(BlocoEntrada *bloco) {
  for(int i = 0; i < f; i++) {
    zeraItem(&bloco->itens[i]);
  }
}

void printaBlocoEntrada(BlocoEntrada bloco) {
  for (int i = 0; i < f; i++) {
    printf("%6.1lf -> %d\n", bloco.itens[i].aluno.nota, bloco.itens[i].marcado);
  } 
}

void iniciaLeitura(Leitura *bloco, int numeroItens) {
  bloco->numeroItens = numeroItens;
  bloco->itensLidos = 0;
}