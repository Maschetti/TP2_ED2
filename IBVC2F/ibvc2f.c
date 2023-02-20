#include "ibvc2f.h"

int comparaItem(Item x, Item y) {
  if(x.aluno.nota > y.aluno.nota && (x.marcado == y.marcado)) return 1;
  if(y.marcado) return 1;
  return 0;
}

void heapBlocoEntrada(BlocoEntrada *bloco) {
  int posicaoPai = 0;
  Item auxiliar = bloco->itens[posicaoPai];
  int posicaoFilho = 1;
  while(posicaoFilho < 10) {
    if(posicaoFilho < 9) {
      if(!comparaItem(bloco->itens[posicaoFilho], bloco->itens[posicaoFilho+1])) posicaoFilho = posicaoFilho + 1;
    }
    if(!comparaItem(auxiliar, bloco->itens[posicaoFilho])) {
      bloco->itens[posicaoPai] = bloco->itens[posicaoFilho];
      posicaoPai = posicaoFilho;
      posicaoFilho = 2 * posicaoPai + 1;
    }
    else posicaoFilho = 10;
  }
  bloco->itens[posicaoPai] = auxiliar;
}

Fita* iniciaFitas() {
  Fita *fitas = malloc(sizeof(Fita) * (2 * f));
  char nomeArquivo[19];
  FILE *arquivo = NULL;
  for(int i = 0; i < (2 * f); i++) {
    snprintf(nomeArquivo, 19, "../Fitas/f_%d.bin", i + 1);
    arquivo = fopen(nomeArquivo, "w+b");
    iniciaFita(&fitas[i], arquivo);
  }
  return fitas;
}

void substituicaoSelecao(FILE *arquivo, Fita *fitas) {
  BlocoEntrada bloco;
  Aluno auxiliar;
  iniciaBlocoEntrada(&bloco);
  for(int i = 0; i < f; i++) {
    fscanfAluno(arquivo, &auxiliar);
    bloco.itens[i].aluno = auxiliar;
  }
}

void IBVC2F(FILE* arquivo) {
  // inicia as fitas
  Fita *fitas = iniciaFitas();
  // for(int i = 0; i < 2 * f; i++) {
  //   printf("%d\n", fitas[i].numeroBlocos);
  // }

  substituicaoSelecao(arquivo, fitas);
    
}