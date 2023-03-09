#include "functions.h"
#include "../Analises/analise.h"

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
      arquivo = fopen("PROVAO_ASCENDENTE.txt", "r");
    }
    else if(situacao == 2) {
      arquivo = fopen("PROVAO_DESCENDENTE.txt", "r");
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

int comparaItem(Item x, Item y) {
  if(x.fim) return 1;
  if(y.fim) return 0;
  if(x.marcado == y.marcado) return (x.aluno.nota > y.aluno.nota);
  if(x.marcado) return 1;
  return 0;
}

int comparaIntercalador(Intercalador x, Intercalador y) {
  if(x.fimArquivo) return 1;
  if(y.fimArquivo) return 0;
  if(x.fimBloco == y.fimBloco) return (y.aluno.nota < x.aluno.nota);
  if(x.fimBloco) return 1;
  return 0;
}

int indexMenorNota(Intercalador *intercaladores, int numeroFitasEntrada, Analise *analise) {
  Intercalador key = intercaladores[0];
  int index = 0;
  for(int i = 1; i < numeroFitasEntrada; i++) {
    atualizaComparacoes_ordenacao(analise, 1);
    if(comparaIntercalador(key, intercaladores[i])) {
      key = intercaladores[i];
      index = i;
    }
  }
  return index;
}

void heapBE(BlocoEntrada *bloco, int posicaoPai, int numeroFitasEntrada, Analise *analise) {
  Item auxiliar = bloco->itens[posicaoPai];
  int posicaoFilho = posicaoPai * 2 + 1;
  while(posicaoFilho < numeroFitasEntrada) {
    if(posicaoFilho < numeroFitasEntrada - 1) {
      atualizaComparacoes_criacao(analise, 1);
      if(!comparaItem(bloco->itens[posicaoFilho + 1], bloco->itens[posicaoFilho])) posicaoFilho++; 
    }
    atualizaComparacoes_criacao(analise, 1);
    if(!comparaItem(bloco->itens[posicaoFilho], auxiliar)) {
      bloco->itens[posicaoPai] = bloco->itens[posicaoFilho];
      posicaoPai = posicaoFilho;
      posicaoFilho = 2 * posicaoPai + 1;
    }
    else posicaoFilho = numeroFitasEntrada;
  }
  bloco->itens[posicaoPai] = auxiliar;
}

void heapBlocoEntrada(BlocoEntrada *bloco, int numeroFitasEntrada, Analise *analise) {
  for(int posicaoPai = (numeroFitasEntrada - 1) / 2; posicaoPai >= 0; posicaoPai--) {
    heapBE(bloco, posicaoPai, numeroFitasEntrada, analise);
  }
}

void printaItem(Item item) {
  printf("M = %d || F = %d\n", item.marcado, item.fim);
}

void substituicaoSelecao(FILE *input, FILE **fitas, int numeroAlunos, int numeroFitasEntrada, Analise *analise) {
  BlocoEntrada bloco;
  bloco.itens = malloc(sizeof(Item) * numeroFitasEntrada);
  int alunosLidos = 0, index = 0;
  //preenche o bloco com os primeiros valores
  for(int i = 0; i < numeroFitasEntrada; i++) {
    fscanfAluno(input, &bloco.itens[i].aluno);
    atualizaLeitura_criacao(analise,1);
    alunosLidos++;

    //aproveito o for para desmarcar e avisar que n esta no fim
    if(i < numeroAlunos) bloco.itens[i].fim = 0;
    else bloco.itens[i].fim = 1;
    bloco.itens[i].marcado = 0;
  }

  //faz o heap no bloco de itens
  heapBlocoEntrada(&bloco, numeroFitasEntrada, analise);

  while(1) {
    //todos itens ja foram printados
    if(bloco.itens[0].fim) break;
    if(bloco.itens[0].marcado) {
      //desmarca todos os itens pois comecou um novo bloco
      for(int i = 0; i < numeroFitasEntrada; i++) bloco.itens[i].marcado = 0;
      //passa para proxima fita
      index = (index + 1) % numeroFitasEntrada;
    }
    fwrite(&bloco.itens[0].aluno, sizeof(Aluno), 1, fitas[index]);
    atualizaEscrita_criacao(analise,1);

    bloco.ultimoInserido = bloco.itens[0].aluno.nota;

    if(alunosLidos == numeroAlunos) bloco.itens[0].fim = 1;
    else {
      fscanfAluno(input, &bloco.itens[0].aluno);
      atualizaLeitura_criacao(analise, 1);
      alunosLidos++;
      atualizaComparacoes_criacao(analise, 1);
      if(bloco.itens[0].aluno.nota < bloco.ultimoInserido) bloco.itens[0].marcado = 1;
    }

    //faz o heap apenas na primeira posicao
    heapBE(&bloco, 0, numeroFitasEntrada, analise);
  }

  free(bloco.itens);
}