#include <stdio.h>
#include <string.h>
#include "Functions/functions.h"
#include "QuickSortExterno/quickSorteExterno.h"
#include "IBVCF1/ibvcf1.h"
#include "IBVC2F/ibvc2f.h"
#include "Tad/tad.h"
#include "Analises/analise.h"

Analise analise;

int main(int numeroArgumentos, char **argumentos) {
  FILE *arquivo = NULL;
  int metodo, quantidade, situacao, P;
  iniciaAnalise(&analise);
  parametrosParaInteiro(numeroArgumentos, argumentos, &metodo, &quantidade, &situacao, &P);
  if((arquivo = validaEntrada(numeroArgumentos, metodo, quantidade, situacao, P)) != NULL) {
    if(metodo == 1) {
      ibvcf1(arquivo, quantidade, &analise);
    }
    else if(metodo == 2) {
      ibvc2f(arquivo, quantidade, &analise);
    }
    else {
      Aluno aluno;
      FILE *input = fopen("PROVAO_BIN.bin", "w+b");
      for(int i = 0; i < quantidade; i++) {
        fscanfAluno(arquivo, &aluno);
        fwrite(&aluno, sizeof(Aluno), 1, input);
      }
      fclose(input);
      iniciaContagemTempo(&analise);
      quickSort("PROVAO_BIN.bin", quantidade, &analise);
      finalizaContagemTempo(&analise);
      atualizaTempo_ordenacao(&analise);
    }
    imprimirDados(&analise);
    fclose(arquivo);
  }
  else {
    printf("ERROR: nao foi possivel abrir o arquivo\n");
  }
}