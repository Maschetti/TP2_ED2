#include <stdio.h>
#include "Functions/functions.h"
#include "IBVC2F/ibvc2f.h"
#include "IBVCF1/ibvcf1.h"
#include "QuickSortExterno/quickSorteExterno.h"
#include "Tad/tad.h"

int main(int numeroArgumentos, char **argumentos) {
  FILE *arquivo = NULL;
  int metodo, quantidade, situacao, P;
  parametrosParaInteiro(numeroArgumentos, argumentos, &metodo, &quantidade, &situacao, &P);
  if((arquivo = validaEntrada(numeroArgumentos, metodo, quantidade, situacao, P)) != NULL) {
    if(metodo == 1) {
      IBVC2F(arquivo, quantidade);
    }
    else if(metodo == 2) {
      IBVCF1();
    }
    else {
      quickSortExterno();
    }

    fclose(arquivo);
  }
  else {
    printf("ERROR: nao foi possivel abrir o arquivo\n");
  }
}