#include "ibvc2f.h"

void startLeituraFitas(Fita *fitas, Leitura *fitasLeitura, int begin) {
  for(int i = 0; i < f; i++) {
    iniciaLeitura(&fitasLeitura[i], fitas[i + begin].numeroBlocos);
  }
}

void fillLeituraBlocos(Fita *fitas, Leitura *fitasLeitura, Leitura *blocosLeitura, int begin) {
  int numeroItens;
  for(int  i = 0; i < f; i++) {
    if(fitasLeitura[i].itensLidos < fitasLeitura[i].numeroItens) fread(&numeroItens, sizeof(int), 1, fitas[i + begin].arquivo);
    else numeroItens = 0;
    blocosLeitura[i].numeroItens = numeroItens;
    blocosLeitura[i].itensLidos = 0;
  }
}

int somaItensBloco(Leitura *blocoLeitura) {
  int soma = 0;
  for(int i = 0; i < f; i++) soma += blocoLeitura[i].numeroItens;
  return soma;
}

int menorNota(Aluno *alunos) {
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

void upFitas(Leitura *fitasLeitura, int begin) {
  for(int i = 0; i < f; i++) {
    if(fitasLeitura[i].itensLidos < fitasLeitura[i].numeroItens) fitasLeitura[i + begin].itensLidos++;
  }
}

int fimIntercalacao(Fita *fitas, int inicioSaida) {
  // printf("%d\n", fitas[inicioSaida].numeroBlocos + fitas[inicioSaida + 1].numeroBlocos);
  if((fitas[inicioSaida].numeroBlocos + fitas[inicioSaida + 1].numeroBlocos) > 1) return 0;
  return 1;
}

void intercalacao2F(Fita *fitas) {
  Leitura *blocosLeitura = malloc(sizeof(Leitura) * f);
  Leitura *fitasLeitura = malloc(sizeof(Leitura) * f);
  Aluno *vetorAlunos = malloc(sizeof(Aluno) * f), aluno;

  int posicaoMenorNota, somaItens;
  int inicioEntrada = 0, inicioSaida = f;

  do {
    ponteirosInicioFitas(fitas);
    // printf("pos = %d\n", inicioEntrada);
    // printf("%d\n", fitas[inicioEntrada].numeroBlocos);
    startLeituraFitas(fitas, fitasLeitura, inicioEntrada);
    // printf("SAIU\n");
    // for(int i = 0; i < f; i++) {
    //   printf("%d ", fitasLeitura[i].numeroItens);
    // }
    // printf("\n");
    // printf("NUMEROBLOCOS pos %d = %d\n", inicioEntrada, fitasLeitura[0].numeroItens);
    for(int index = 0; fitasLeitura[0].numeroItens > index; index++) {
      // printf("BLOCO => %d\n", index + 1);
      fillLeituraBlocos(fitas, fitasLeitura, blocosLeitura, inicioEntrada);
      // for(int i = 0; i < f; i++) {
      //   printf("itens %d || lidos %d\n", blocosLeitura[i].numeroItens, blocosLeitura[i].itensLidos);
      // }
      // printf("\n");
      // getc(stdin);

      for(int i = 0; i < f; i++) {
        if(blocosLeitura[i].numeroItens != blocosLeitura[i].itensLidos) {
          freadAluno(fitas[i + inicioEntrada].arquivo, &vetorAlunos[i]);
          blocosLeitura[i].itensLidos++;
        }
        else vetorAlunos[i].nota = -1; 
      }

      somaItens = somaItensBloco(blocosLeitura);
      fwrite(&somaItens, sizeof(int), 1, fitas[inicioSaida + (index % f)].arquivo);
      // printf("SOMA %d\n", somaItens);
      // printf("FITA SAIDA ATUAL => %d\n",inicioSaida + (index % f));

      for(int i = 0; i < somaItens; i++) {
        posicaoMenorNota = menorNota(vetorAlunos);
        fwriteAluno(vetorAlunos[posicaoMenorNota], fitas[inicioSaida + (index % f)].arquivo);
        if(blocosLeitura[posicaoMenorNota].itensLidos != blocosLeitura[posicaoMenorNota].numeroItens) {
          freadAluno(fitas[inicioEntrada + posicaoMenorNota].arquivo, &vetorAlunos[posicaoMenorNota]);
          // printf("POSICAO VETOR => %d\n", inicioEntrada + posicaoMenorNota);
          blocosLeitura[posicaoMenorNota].itensLidos++;
        }
        else vetorAlunos[posicaoMenorNota].nota = -1;
      }

      upFitas(fitasLeitura, inicioEntrada);
      // for(int i = 0; i < f; i++) {
      //   printf("Lidos %d || itens %d\n", fitasLeitura[i + inicioEntrada].itensLidos, fitasLeitura[i + inicioEntrada].numeroItens);
      // }
      fitas[inicioSaida + (index % f)].numeroBlocos++;
      // printf("FITA SAIDA ATUAL => %d\n",inicioSaida + (index % f));
    }
    for(int i = 0; i < f; i++) {
      fitas[i + inicioEntrada].numeroBlocos = 0;
    }
    // for(int i = 0; i < 2 * f; i++) {
    //   printf("FITA %d => %d\n", i + 1, fitas[i].numeroBlocos);
    // }
    // getc(stdin);
    inicioEntrada = inicioSaida;
    inicioSaida = (inicioEntrada == f ? 0 : f);
  } while (!fimIntercalacao(fitas, inicioEntrada));

  fseek(fitas[inicioEntrada].arquivo, 0, SEEK_SET);
  FILE *file = fopen("ascendente.txt", "w");
  fread(&somaItens, sizeof(int), 1, fitas[inicioEntrada].arquivo);
  for(int j = 0; j < somaItens; j++) {
    freadAluno(fitas[inicioEntrada].arquivo, &aluno);
    fprintAluno(aluno, file);
  }
  fclose(file);

  // free(blocosLeitura);
  // free(fitasLeitura);
  // free(vetorAlunos);
}

void ibvc2f(FILE* arquivo, int numeroAlunos) {
  Fita *fitas = iniciaFitas();
  substituicaoSelecao(arquivo, fitas, numeroAlunos, f);
  // for(int i = 0; i < 2 * f; i++) {
  //   printf("FITA %d => %d\n", i + 1, fitas[i].numeroBlocos);
  // }
  intercalacao2F(fitas);
  free(fitas);
}