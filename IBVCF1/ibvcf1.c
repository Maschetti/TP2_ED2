#include "ibvcf1.h"

void preencheBlocosLeitura(Leitura *blocos, Fita *fitas, Leitura *fitasLeitura, int numeroFitasEntrada) {
  int aux;
  for(int i = 0; i < numeroFitasEntrada; i++) {
    if(fitasLeitura[i].itensLidos < fitasLeitura[i].numeroItens) fread(&aux, sizeof(int), 1, fitas[i].arquivo);
    else aux = 0;
    iniciaLeitura(&blocos[i], aux);
  }
}

int continuarIntercalando(Leitura *fitasLeitura, int index, int numeroFitasEntrada) {
  for(int i = 0; i < numeroFitasEntrada; i++) {
    if(fitasLeitura[i].numeroItens > index) return 1;
  }
  return 0;
}

void printaBlocosLeitura(Leitura *blocos, int numeroFitasEntrada) {
  for(int i = 0; i < numeroFitasEntrada; i++) {
    printf("itens = %d || lidos = %d\n", blocos[i].numeroItens, blocos[i].itensLidos);
  }
}

int indexMenorNota(Aluno *alunos, int numeroFitasEntrada) {
  int index, i;
  double menorNota = 101;
  for(i = 0; i < numeroFitasEntrada; i++) {
    if((alunos[i].nota < menorNota) && alunos[i].nota != -1) {
      index = i;
      menorNota = alunos[i].nota; 
    }
  }
  return index;
}

int numeroItensTodosBlocos(Leitura *bloco, int numeroFitasEntrada) {
  int soma = 0;
  for(int i = 0; i < numeroFitasEntrada; i++) {
    soma += bloco[i].numeroItens;
  }
  return soma;
}

// void printaBloco(FILE* arquivo) {
//   int itens;
//   Aluno aux;
//   fread(&itens, sizeof(int), 1, arquivo);
//   printf("%d\n", itens);
//   for(int i = 0; i < itens; i++) {
//     freadAluno(arquivo, &aux);
//     printf("%d -> ", i + 1);
//     printAluno(aux);
//   }
// }

void incrementaFitasLeitura(Leitura *fitasLeitura, int numeroFitasEntrada) {
  for(int i = 0; i < numeroFitasEntrada; i++) {
    if(fitasLeitura[i].itensLidos < fitasLeitura[i].numeroItens) fitasLeitura[i].itensLidos++;
  }
}

int realocaBlocos(Fita *fitas, int numeroFitasEntrada) {
  if(fitas[numeroFitasEntrada].numeroBlocos == 1) return 0;

  int numeroItens;
  Aluno aluno;
  for(int i = 0; i < numeroFitasEntrada; i++) {
    fitas[i].numeroBlocos = 0;
    if(i < fitas[numeroFitasEntrada].numeroBlocos) {
      fitas[i].numeroBlocos++;
      fread(&numeroItens, sizeof(int), 1, fitas[numeroFitasEntrada].arquivo);
      fwrite(&numeroItens, sizeof(int), 1, fitas[i].arquivo);

      for(int j = 0; j < numeroItens; j++) {
        freadAluno(fitas[numeroFitasEntrada].arquivo, &aluno);
        fwriteAluno(aluno, fitas[i].arquivo);
      }
    }
  }

  fitas[numeroFitasEntrada].numeroBlocos = 0;
  return 1;
}

void intercalacaoF1(Fita *fitas, int numeroFitasEntrada) {
  //aloca blocos para leitura
  Leitura *blocosLeitura = malloc(sizeof(Leitura) * numeroFitasEntrada);
  Leitura *fitasLeitura = malloc(sizeof(Leitura) * numeroFitasEntrada);
  Aluno *vetorAlunos = malloc(sizeof(Aluno) * numeroFitasEntrada), aluno;
  int posicaoMenorNota, somaItens;
  // printaBlocosLeitura(blocosLeitura);

  do {
    ponteirosInicioFitas(fitas);
    for(int i = 0; i < numeroFitasEntrada; i++) iniciaLeitura(&fitasLeitura[i], fitas[i].numeroBlocos);

    for(int index = 0; continuarIntercalando(fitasLeitura, index, numeroFitasEntrada); index++) {
      preencheBlocosLeitura(blocosLeitura, fitas, fitasLeitura, numeroFitasEntrada);
      
      //preenche o vetor para comecar a ordenar
      for(int i = 0; i < numeroFitasEntrada; i++) {
        if(blocosLeitura[i].numeroItens != blocosLeitura[i].itensLidos) {
          freadAluno(fitas[i].arquivo, &vetorAlunos[i]);
          blocosLeitura[i].itensLidos++;
        }
        else vetorAlunos[i].nota = -1;
      }

      somaItens = numeroItensTodosBlocos(blocosLeitura, numeroFitasEntrada);
      fwrite(&somaItens, sizeof(int), 1, fitas[numeroFitasEntrada].arquivo);

      for(int i = 0; i < somaItens; i++) {
        posicaoMenorNota = indexMenorNota(vetorAlunos, numeroFitasEntrada);
        fwriteAluno(vetorAlunos[posicaoMenorNota], fitas[numeroFitasEntrada].arquivo);
        if(blocosLeitura[posicaoMenorNota].itensLidos != blocosLeitura[posicaoMenorNota].numeroItens) {
          freadAluno(fitas[posicaoMenorNota].arquivo, &vetorAlunos[posicaoMenorNota]);
          blocosLeitura[posicaoMenorNota].itensLidos++;
        }
        else vetorAlunos[posicaoMenorNota].nota = -1;
      }

      incrementaFitasLeitura(fitasLeitura, numeroFitasEntrada);
      fitas[numeroFitasEntrada].numeroBlocos++;
    }
    ponteirosInicioFitas(fitas);
  } while(realocaBlocos(fitas, numeroFitasEntrada));
  

  

  fseek(fitas[numeroFitasEntrada].arquivo, 0, SEEK_SET);
  FILE *file = fopen("ascendente.txt", "w");
  for(int i = 0; i < fitas[numeroFitasEntrada].numeroBlocos; i++) {
    fread(&somaItens, sizeof(int), 1, fitas[numeroFitasEntrada].arquivo);
    for(int j = 0; j < somaItens; j++) {
      freadAluno(fitas[numeroFitasEntrada].arquivo, &aluno);
      fprintAluno(aluno, file);
    }
  }
  fclose(file);
  free(blocosLeitura);
  free(fitasLeitura);
  free(vetorAlunos);
}

void ibvcf1(FILE* arquivo, int numeroAlunos) {
  Fita *fitas = iniciaFitas();
  substituicaoSelecao(arquivo, fitas, numeroAlunos, 2 * f - 1);
  intercalacaoF1(fitas, 2 * f - 1);
}