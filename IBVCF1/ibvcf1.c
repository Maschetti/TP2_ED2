#include "ibvcf1.h"
#include "../Analises/analise.h"

int realocaBlocos(int index, FILE **fitas, int numeroFitasEntrada, Intercalador *intercaladores, Analise *analise) {
  if(index == 1) return 0;

  char nomeArquivo[20];
  for(int i = 0; i < numeroFitasEntrada; i++) {
    fclose(fitas[i]);

    snprintf(nomeArquivo, 20, "Fitas/f_%d.bin", i + 1);
    remove(nomeArquivo);
    fitas[i] = fopen(nomeArquivo, "w+b");
    fseek(fitas[i], 0, SEEK_SET);
  }

  Aluno aluno;
  double notaAnterior = -1;
  int indexFita = 0;
  fseek(fitas[numeroFitasEntrada], 0, SEEK_SET);
  atualizaLeitura_ordenacao(analise, 1);
  while(fread(&aluno, sizeof(Aluno), 1, fitas[numeroFitasEntrada]) != 0) {
    atualizaLeitura_ordenacao(analise, 1);
    atualizaComparacoes_ordenacao(analise, 1);
    if(aluno.nota < notaAnterior) indexFita = (indexFita + 1) % numeroFitasEntrada;

    atualizaEscrita_ordenacao(analise, 1);
    fwrite(&aluno, sizeof(Aluno), 1, fitas[indexFita]);
    notaAnterior = aluno.nota;
  }

  fclose(fitas[numeroFitasEntrada]);
  snprintf(nomeArquivo, 20, "Fitas/f_%d.bin", numeroFitasEntrada + 1);
  remove(nomeArquivo);
  fitas[numeroFitasEntrada] = fopen(nomeArquivo, "w+b");

  return 1;
}

void intercalacaoF1(FILE **fitas, int numeroFitasEntrada, Analise *analise) {
  Intercalador *intercaladores = malloc(sizeof(Intercalador) * numeroFitasEntrada);
  Aluno aluno;
  int index, pos;

  do {
    for (int i = 0; i < numeroFitasEntrada; i++) {
      fseek(fitas[i], 0, SEEK_SET);
    }
    index = 0;

    for(int i = 0; i < numeroFitasEntrada; i++) {
      atualizaLeitura_ordenacao(analise, 1);
      if(fread(&intercaladores[i].aluno, sizeof(Aluno), 1, fitas[i]) == 0) {
        intercaladores[i].fimArquivo = 1;
      }
      else intercaladores[i].fimArquivo = 0;
      intercaladores[i].fimBloco = 0;
    }

    while(1) {
      //fim dos arquivos
      pos = indexMenorNota(intercaladores, numeroFitasEntrada, analise);
      
      if(intercaladores[pos].fimArquivo) {
        index++;
        break;
      }
      if(intercaladores[pos].fimBloco) {
        //passa pro proximo bloco
        for(int i = 0; i < numeroFitasEntrada; i++) intercaladores[i].fimBloco = 0;
        index++;
      }
      atualizaEscrita_ordenacao(analise, 1);
      fwrite(&intercaladores[pos].aluno, sizeof(Aluno), 1, fitas[numeroFitasEntrada]);

      atualizaLeitura_ordenacao(analise, 1);
      if(fread(&aluno, sizeof(Aluno), 1, fitas[pos]) == 0) {
        intercaladores[pos].fimArquivo = 1;
      }
      else {  
        atualizaComparacoes_ordenacao(analise, 1);
        if(aluno.nota < intercaladores[pos].aluno.nota) intercaladores[pos].fimBloco = 1;
        intercaladores[pos].aluno = aluno;
      }
    }
    
  } while(realocaBlocos(index, fitas, numeroFitasEntrada, intercaladores, analise));

  fseek(fitas[numeroFitasEntrada], 0, SEEK_SET);
  FILE *output = fopen("ascendente.txt", "w");

  while(fread(&aluno, sizeof(Aluno), 1, fitas[numeroFitasEntrada]) != 0) {
    fprintAluno(aluno, output);
    atualizaLeitura_ordenacao(analise, 1);
  }
  atualizaLeitura_ordenacao(analise, 1);

  fclose(output);
  free(intercaladores);
}

void ibvcf1(FILE *input, int numeroAlunos, Analise *analise) {
  //cria as fitas
  FILE **fitas = malloc(sizeof(fitas) * 2 * f);
  char nomeArquivo[20];
  for(int i = 0; i < 2 * f; i++) {
    snprintf(nomeArquivo, 20, "Fitas/f_%d.bin", i + 1);
    fitas[i] = fopen(nomeArquivo, "w+b");
  }
  iniciaContagemTempo(analise);
  substituicaoSelecao(input, fitas, numeroAlunos, 2 * f - 1, analise);
  finalizaContagemTempo(analise);
  atualizaTempo_criacao(analise);

  for(int i = 0; i < 2 * f; i++) {
    fseek(fitas[i], 0, SEEK_SET);
  }

  iniciaContagemTempo(analise);
  intercalacaoF1(fitas, 2 * f - 1, analise);
  finalizaContagemTempo(analise);
  atualizaTempo_ordenacao(analise);
  //libera os arquivos das fitas
  for(int i = 0; i < 2 * f; i++) {
    fclose(fitas[i]);
  }
  free(fitas);
}