#include "ibvc2f.h"

int comparaItem(Item x, Item y) {
  if((x.aluno.nota > y.aluno.nota) && (x.marcado == y.marcado)) return 1;
  if(x.marcado) return 1;
  return 0;
}

void heapBE(BlocoEntrada *bloco, int posicaoPai) {
  Item auxiliar = bloco->itens[posicaoPai];
  int posicaoFilho = posicaoPai * 2 + 1;
  while(posicaoFilho < f) {
    if(posicaoFilho < f - 1) {
      if(!comparaItem(bloco->itens[posicaoFilho + 1], bloco->itens[posicaoFilho])) posicaoFilho++; 
    }
    if(!comparaItem(bloco->itens[posicaoFilho], auxiliar)) {
      bloco->itens[posicaoPai] = bloco->itens[posicaoFilho];
      posicaoPai = posicaoFilho;
      posicaoFilho = 2 * posicaoPai + 1;
    }
    else posicaoFilho = f;
  }
  bloco->itens[posicaoPai] = auxiliar;
}

void heapBlocoEntrada(BlocoEntrada *bloco) {
  for(int posicaoPai = (f - 1) / 2; posicaoPai >= 0; posicaoPai--) {
    heapBE(bloco, posicaoPai);
  }
}

Fita* iniciaFitas() {
  Fita *fitas = malloc(sizeof(Fita) * (2 * f));
  char nomeArquivo[16];
  FILE *arquivo = NULL;
  for(int i = 0; i < (2 * f); i++) {
    snprintf(nomeArquivo, 16, "Fitas/f_%d.txt", i + 1);
    arquivo = fopen(nomeArquivo, "w+b");
    iniciaFita(&fitas[i], arquivo);
  }
  return fitas;
}

int todosMarcados(BlocoEntrada bloco) {
  for(int i = 0; i < f; i++) {
    if(bloco.itens[i].marcado == 0) return 0;
  }
  return 1;
}

void substituicaoSelecao(FILE *arquivo, Fita *fitas) {
  BlocoEntrada bloco;
  Aluno aluno;
  Item item;
  zeraBlocoEntrada(&bloco);
  // preenche o bloco
  for(int i = 0; i < f; i++) {
    fscanfAluno(arquivo, &aluno);
    bloco.itens[i].aluno = aluno;
  }

  for(int i = 0; i < f; i++) {
    heapBlocoEntrada(&bloco);
    do {
      fprintAluno(bloco.itens[0].aluno, fitas[i].arquivo);
      bloco.ultimoInserido = bloco.itens[0].aluno.nota;
      fscanfAluno(arquivo, &aluno);

      item.aluno = aluno;
      if(item.aluno.nota < bloco.ultimoInserido) {
        item.marcado = 1;
      }
      else zeraItem(&item);

      bloco.itens[0] = item;
      heapBE(&bloco, 0);
    } while(!todosMarcados(bloco));
    zeraBlocoEntrada(&bloco);
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