#ifndef analise_h
#define analise_h

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//a analise eh dormado por numero de transferencias, comparacoes e o tempo
typedef struct{
  
  int leitura_ordenacao, leitura_criacao;
  int escrita_ordenacao, escrita_criacao;
  int comparacoes_criacao, comparacoes_ordenacao;
  double tempo_ordenacao, tempo_criacao;

  clock_t iniTempo, fimTempo;
} Analise;


//funcao que inicia as analises
void iniciaAnalise(Analise *analise);

//funcao que imprime os dados obtidos na analise 
void imprimirDados(Analise *analise);

//funcoes para contagem do tempo 
void iniciaContagemTempo(Analise *analise);
void finalizaContagemTempo(Analise *analise);

//funcoes para numero de transfencias
void atualizaLeitura_criacao(Analise *analise, int qtd);
void atualizaLeitura_ordenacao(Analise *analise, int qtd);
void atualizaEscrita_criacao(Analise *analise, int qtd);
void atualizaEscrita_ordenacao(Analise *analise, int qtd);

//funcoes para numero de comparacoes
void atualizaComparacoes_criacao(Analise *analise, int qtd);
void atualizaComparacoes_ordenacao(Analise *analise, int qtd);

//funcoes para atualizacao do tempo de contagem
void atualizaTempo_criacao(Analise *analise);
void atualizaTempo_ordenacao(Analise *analise);

#endif