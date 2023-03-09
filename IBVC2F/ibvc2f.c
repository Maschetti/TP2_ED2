#include "ibvc2f.h"
#include "../Analises/analise.h"

void inter(FILE **fitas, Intercalador *intercaladores, int inicioEntrada, int inicioSaida, Analise *analise) {
  for(int i = 0; i < f; i++) {
    atualizaLeitura_ordenacao(analise, 1);
    if(fread(&intercaladores[i].aluno, sizeof(Aluno), 1, fitas[i + inicioEntrada]) == 0) {
      intercaladores[i].fimArquivo = 1;
    }
    else intercaladores[i].fimArquivo = 0;
    intercaladores[i].fimBloco = 0;
  }

  int pos, posSaida = 0;
  Aluno aluno;
  while(1) {
    pos = indexMenorNota(intercaladores, f, analise);

    if(intercaladores[pos].fimArquivo) break;

    if(intercaladores[pos].fimBloco) {
      //passa pro proximo bloco
      for(int i = 0; i < f; i++) intercaladores[i].fimBloco = 0;
      posSaida = (posSaida + 1) % f;
    }

    atualizaEscrita_ordenacao(analise, 1);
    fwrite(&intercaladores[pos].aluno, sizeof(Aluno), 1, fitas[posSaida + inicioSaida]);

    atualizaLeitura_ordenacao(analise, 1);
    if(fread(&aluno, sizeof(Aluno), 1, fitas[pos + inicioEntrada]) == 0) {
      intercaladores[pos].fimArquivo = 1;
    }
    else {  
      atualizaComparacoes_ordenacao(analise, 1);
      if(aluno.nota < intercaladores[pos].aluno.nota) intercaladores[pos].fimBloco = 1;
      intercaladores[pos].aluno = aluno;
    }
  }
}

void intercalacao2F(FILE **fitas, Analise *analise) {
  Intercalador *intercaladores = malloc(sizeof(Intercalador) * f);
  int inicioEntrada = f, inicioSaida = 0;
  Aluno aluno;

  do {
    for (int i = 0; i < 2 * f; i++) fseek(fitas[i], 0, SEEK_SET);

    inicioEntrada = inicioSaida;
    inicioSaida = (inicioEntrada == 0 ? f : 0); 
    inter(fitas, intercaladores, inicioEntrada, inicioSaida, analise);

    char nomeArquivo[20];
    for(int i = inicioEntrada; i < inicioEntrada + f; i++) {
      fclose(fitas[i]);

      snprintf(nomeArquivo, 20, "Fitas/f_%d.bin", i + 1);
      remove(nomeArquivo);
      fitas[i] = fopen(nomeArquivo, "w+b");
      fseek(fitas[i], 0, SEEK_SET);
    }

    fseek(fitas[inicioSaida + 1], 0 , SEEK_SET);
    atualizaLeitura_ordenacao(analise, 1);
  } while(fread(&aluno, sizeof(Aluno), 1, fitas[inicioSaida + 1]) != 0);

  fseek(fitas[inicioSaida], 0 , SEEK_SET);
  FILE *output = fopen("ascendente.txt", "w");
  atualizaLeitura_ordenacao(analise, 1);
  while(fread(&aluno, sizeof(Aluno), 1, fitas[inicioSaida]) != 0) {
    fprintAluno(aluno, output);
    atualizaLeitura_ordenacao(analise, 1);
  }

  fclose(output);
  free(intercaladores);

}

void ibvc2f(FILE *input, int numeroAlunos, Analise *analise) {
  //cria as fitas
  FILE **fitas = malloc(sizeof(fitas) * 2 * f);
  char nomeArquivo[20];
  for(int i = 0; i < 2 * f; i++) {
    snprintf(nomeArquivo, 20, "Fitas/f_%d.bin", i + 1);
    fitas[i] = fopen(nomeArquivo, "w+b");
  }

  iniciaContagemTempo(analise);
  substituicaoSelecao(input, fitas, numeroAlunos, f, analise);
  finalizaContagemTempo(analise);
  atualizaTempo_criacao(analise);

  for(int i = 0; i < 2 * f; i++) {
    fseek(fitas[i], 0, SEEK_SET);
  }

  iniciaContagemTempo(analise);
  intercalacao2F(fitas, analise);
  finalizaContagemTempo(analise);
  atualizaTempo_ordenacao(analise);

  //libera os arquivos das fitas
  for(int i = 0; i < 2 * f; i++) {
    fclose(fitas[i]);
  }
  free(fitas);
}