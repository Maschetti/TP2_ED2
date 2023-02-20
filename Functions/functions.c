#include "functions.h"

void imprimeArquivo(FILE* arquivo, int numeroItens) {
  Aluno auxiliar;
  for(int i = 0; i < numeroItens; i++) {
    fscanfAluno(arquivo, &auxiliar);
    printAluno(auxiliar);
  }
}

void parametrosParaInteiro(int numeroArgumentos, char **argumentos, int *metodo, int *quantidade, int *situacao, int *P) {
  if(numeroArgumentos >= 4 && numeroArgumentos < 6) { 
    *metodo = atoi(argumentos[1]);
    *quantidade = atoi(argumentos[2]);
    *situacao = atoi(argumentos[3]);
    *P = 0;

    if(numeroArgumentos == 5) {
      if(strcmp(argumentos[4], "-P") == 0) {
        *P = 1;
      }
    }
  }
  else {
    printf("ERROR: Numero de argumentos invalidos\n");
    printf("Entrada: pesquisa <metodo> <quantidade> <situacao> [-P]\n");
    printf("OBS: apenas o parametro [-P] e opcional\n");
  }
}

FILE* validaEntrada(int numeroArgumentos, int metodo, int quantidade, int situacao, int P) {
  FILE* arquivo = NULL;
  int valido = 1;

  if(metodo < 1 || metodo > 4) {
    printf("ERROR: metodo invalido\n");
    printf("OBS: metodo deve ser de 1 a 3\n\n");
    valido = 0;
  }

  if(quantidade < 1 || quantidade > 471705) {
    printf("ERROR: quantidade invalida\n");
    printf("OBS: o arquivo deve conter de 1 a 471.705 de registros\n\n");
    valido = 0;
  }

  if(situacao < 1 || situacao > 3) {
    printf("ERROR: situacao invalida\n");
    printf("OBS: a situacao do arquivo pode ser (1)ascendente (2)descendente (3)desordenado\n\n");
    valido = 0;
  }

  if(valido) {
    if(situacao == 1) {
      arquivo = fopen("ascendente.txt", "r");
    }
    else if(situacao == 2) {
      arquivo = fopen("descendente.txt", "r");
    }
    else {
      arquivo = fopen("PROVAO.txt", "r");
    }
  }

  if(P == 1) {
    imprimeArquivo(arquivo, quantidade);
  }

  return arquivo;
}