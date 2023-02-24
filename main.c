#include <stdio.h>
#include "Functions/functions.h"
#include "Intercalacao/intercalacao.h"
#include "QuickSortExterno/quickSorteExterno.h"
#include "Tad/tad.h"

int main(int numeroArgumentos, char **argumentos) {
  FILE *arquivo = NULL;
  int metodo, quantidade, situacao, P;
  parametrosParaInteiro(numeroArgumentos, argumentos, &metodo, &quantidade, &situacao, &P);
  if((arquivo = validaEntrada(numeroArgumentos, metodo, quantidade, situacao, P)) != NULL) {
    if(metodo == 3) {
      quickSortExterno();
    }
    else {
      intercalacao(arquivo, quantidade, metodo);
    }

    fclose(arquivo);
  }
  else {
    printf("ERROR: nao foi possivel abrir o arquivo\n");
  }
}