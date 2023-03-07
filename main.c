#include <stdio.h>
#include "Functions/functions.h"
#include "IBVCF1/ibvcf1.h"
#include "QuickSortExterno/quickSorteExterno.h"
#include "Tad/tad.h"

int main(int numeroArgumentos, char **argumentos) {
  FILE *arquivo = NULL;
  int metodo, quantidade, situacao, P;
  parametrosParaInteiro(numeroArgumentos, argumentos, &metodo, &quantidade, &situacao, &P);
  if((arquivo = validaEntrada(numeroArgumentos, metodo, quantidade, situacao, P)) != NULL) {
    if(metodo == 2) {
      ibvcf1(arquivo, quantidade);
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