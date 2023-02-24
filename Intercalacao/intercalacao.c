#include "intercalacao.h"

void ponteirosInicioFitas(Fita *fitas) {
  for(int i = 0; i < f; i++) {
    fseek(fitas[i].arquivo, 0, SEEK_SET);
  }
}

int fimIntercalacaoF1(Fita *fitas) {
  return (fitas[f].numeroBlocos == 1);
}

void preencheBlocosLeitura(BlocoLeitura *blocos, Fita *fitas, FitaLeitura *fitasLeitura) {
  int aux;
  for(int i = 0; i < f; i++) {
    if(fitasLeitura[i].blocosLidos < fitasLeitura[i].numeroBlocos) fread(&aux, sizeof(int), 1, fitas[i].arquivo);
    else aux = 0;
    iniciaBlocoLeitura(&blocos[i], aux);
  }
}

int continuarIntercalando(Fita *fitas, int index) {
  for(int i = 0; i < f; i++) {
    if(fitas[i].numeroBlocos > index) return 1;
  }
  return 0;
}

void printaBlocosLeitura(BlocoLeitura *blocos) {
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

int numeroItensTodosBlocos(BlocoLeitura *bloco) {
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

void incrementaFitasLeitura(FitaLeitura *fitasLeitura) {
  for(int i = 0; i < f; i++) {
    if(fitasLeitura[i].blocosLidos < fitasLeitura[i].numeroBlocos) fitasLeitura[i].blocosLidos++;
  }
}

void IBVCF1(Fita *fitas) {
  //aloca blocos para leitura
  BlocoLeitura *blocos = malloc(sizeof(BlocoLeitura) * f);
  FitaLeitura *fitasLeitura = malloc(sizeof(FitaLeitura) * f);
  Aluno vetorAlunos[f];
  int posicaoMenorNota, somaItens;
  // printaBlocosLeitura(blocos);

  for(int i = 0; i < f; i++) {
    fitasLeitura[i].numeroBlocos = fitas[i].numeroBlocos;
    fitasLeitura[i].blocosLidos = 0;
  }

  for(int index = 0; continuarIntercalando(fitas, index); index++) {
    preencheBlocosLeitura(blocos, fitas, fitasLeitura);
    
    //preenche o vetor para comecar a ordenar
    for(int i = 0; i < f; i++) {
      if(blocos[i].numeroItens != blocos[i].itensLidos) {
        freadAluno(fitas[i].arquivo, &vetorAlunos[i]);
        blocos[i].itensLidos++;
      }
      else vetorAlunos[i].nota = -1;
    }

    somaItens = numeroItensTodosBlocos(blocos);
    fwrite(&somaItens, sizeof(int), 1, fitas[f].arquivo);

    for(int i = 0; i < somaItens; i++) {
      posicaoMenorNota = indexMenorNota(vetorAlunos);
      fwriteAluno(vetorAlunos[posicaoMenorNota], fitas[f].arquivo);
      if(blocos[posicaoMenorNota].itensLidos != blocos[posicaoMenorNota].numeroItens) {
        freadAluno(fitas[posicaoMenorNota].arquivo, &vetorAlunos[posicaoMenorNota]);
        blocos[posicaoMenorNota].itensLidos++;
      }
      else vetorAlunos[posicaoMenorNota].nota = -1;
    }

    incrementaFitasLeitura(fitasLeitura);
  }

  

  fseek(fitas[f].arquivo, 0, SEEK_SET);
  printaBloco(fitas[f].arquivo);
  free(blocos);
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