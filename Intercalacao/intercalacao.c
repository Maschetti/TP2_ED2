#include "intercalacao.h"

void ponteirosInicioFitas(Fita *fitas) {
  for(int i = 0; i < f; i++) {
    fseek(fitas[i].arquivo, 0, SEEK_SET);
  }
}

int fimIntercalacaoF1(Fita *fitas) {
  return (fitas[f].numeroBlocos == 1);
}

void preencheBlocosLeitura(Leitura *blocos, Fita *fitas, Leitura *fitasLeitura) {
  int aux;
  for(int i = 0; i < f; i++) {
    if(fitasLeitura[i].itensLidos < fitasLeitura[i].numeroItens) fread(&aux, sizeof(int), 1, fitas[i].arquivo);
    else aux = 0;
    iniciaLeitura(&blocos[i], aux);
  }
}

int continuarIntercalando(Leitura *fitasLeitura, int index) {
  for(int i = 0; i < f; i++) {
    if(fitasLeitura[i].numeroItens > index) return 1;
  }
  return 0;
}

void printaBlocosLeitura(Leitura *blocos) {
  for(int i = 0; i < f; i++) {
    printf("itens = %d || lidos = %d\n", blocos[i].numeroItens, blocos[i].itensLidos);
  }
}

int indexMenorNota(Aluno *alunos) {
  int index, i;
  double menorNota = 101;
  for(i = 0; i < f; i++) {
    if((alunos[i].nota < menorNota) && alunos[i].nota != -1) {
      index = i;
      menorNota = alunos[i].nota; 
    }
  }
  return index;
}

int numeroItensTodosBlocos(Leitura *bloco) {
  int soma = 0;
  for(int i = 0; i < f; i++) {
    soma += bloco[i].numeroItens;
  }
  return soma;
}

void printaBloco(FILE* arquivo) {
  int itens;
  Aluno aux;
  fread(&itens, sizeof(int), 1, arquivo);
  printf("%d\n", itens);
  for(int i = 0; i < itens; i++) {
    freadAluno(arquivo, &aux);
    printf("%d -> ", i + 1);
    printAluno(aux);
  }
}

void incrementaFitasLeitura(Leitura *fitasLeitura) {
  for(int i = 0; i < f; i++) {
    if(fitasLeitura[i].itensLidos < fitasLeitura[i].numeroItens) fitasLeitura[i].itensLidos++;
  }
}

void IBVCF1(Fita *fitas) {
  //aloca blocos para leitura
  Leitura *blocosLeitura = malloc(sizeof(Leitura) * f);
  Leitura *fitasLeitura = malloc(sizeof(Leitura) * f);
  Aluno vetorAlunos[f];
  int posicaoMenorNota, somaItens;
  // printaBlocosLeitura(blocosLeitura);

  // for(int i = 0; i < f; i++) iniciaLeitura(&fitasLeitura[i], fitas[i].numeroBlocos);

  // for(int index = 0; continuarIntercalando(fitasLeitura, index); index++) {
  //   preencheBlocosLeitura(blocosLeitura, fitas, fitasLeitura);
    
  //   //preenche o vetor para comecar a ordenar
  //   for(int i = 0; i < f; i++) {
  //     if(blocosLeitura[i].numeroItens != blocosLeitura[i].itensLidos) {
  //       freadAluno(fitas[i].arquivo, &vetorAlunos[i]);
  //       blocosLeitura[i].itensLidos++;
  //     }
  //     else vetorAlunos[i].nota = -1;
  //   }

  //   somaItens = numeroItensTodosBlocos(blocosLeitura);
  //   fwrite(&somaItens, sizeof(int), 1, fitas[f].arquivo);

  //   for(int i = 0; i < somaItens; i++) {
  //     posicaoMenorNota = indexMenorNota(vetorAlunos);
  //     fwriteAluno(vetorAlunos[posicaoMenorNota], fitas[f].arquivo);
  //     if(blocosLeitura[posicaoMenorNota].itensLidos != blocosLeitura[posicaoMenorNota].numeroItens) {
  //       freadAluno(fitas[posicaoMenorNota].arquivo, &vetorAlunos[posicaoMenorNota]);
  //       blocosLeitura[posicaoMenorNota].itensLidos++;
  //     }
  //     else vetorAlunos[posicaoMenorNota].nota = -1;
  //   }

  //   incrementaFitasLeitura(fitasLeitura);
  //   fitas[f].numeroBlocos++;
  // }

  

  fseek(fitas[1].arquivo, 0, SEEK_SET);
  for(int i = 0; i < fitas[1].numeroBlocos; i++) {
    printaBloco(fitas[1].arquivo);
  }
  free(blocosLeitura);
  free(fitasLeitura);
}

void IBVC2F(Fita *fitas) {
  
}

void intercalacao(FILE* arquivo, int numeroAlunos, int qualIntercalacao) {
  // inicia as fitas
  int numeroFitas = (qualIntercalacao == 1 ? (2 * f) : (f + 1));
  Fita *fitas = iniciaFitas(numeroFitas);
  // coloca os valores de entrada na fita
  substituicaoSelecao(arquivo, fitas, numeroAlunos);

  ponteirosInicioFitas(fitas);

  if(qualIntercalacao == 1) IBVC2F(fitas);
  else IBVCF1(fitas);

  free(fitas);
}