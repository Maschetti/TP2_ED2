#include "analise.h"

//funcoes para analises solicitadas
// aqui faz-se a analise para as etapas de criacao dos indices 
// e a analise para a ordenacao em si
void iniciaAnalise(Analise *analise){
  //depois -> analise da ordenacao
  analise->leitura_ordenacao = 0; 
  analise->escrita_ordenacao = 0; 
  analise->comparacoes_ordenacao = 0;

  //antes -> analise da criacao 
  analise->leitura_criacao = 0; 
  analise->escrita_criacao = 0; 
  analise->comparacoes_criacao = 0;
}

//funcoes para contagem do tempo de execucao
//inicia e finaliza o clock - reloginho
void iniciaContagemTempo(Analise *analise){
  analise->iniTempo = clock();
}

void finalizaContagemTempo(Analise *analise){
  analise->fimTempo = clock();
}

//funcoes para verificacao do numero de transferencias
void atualizaLeitura_ordenacao(Analise *analise, int qtd){
  analise->leitura_ordenacao += qtd;
}

void atualizaLeitura_criacao(Analise *analise, int qtd){
  analise->leitura_criacao += qtd;
}

void atualizaEscrita_ordenacao(Analise *analise, int qtd){
  analise->escrita_ordenacao += qtd;
}

void atualizaEscrita_criacao(Analise *analise, int qtd){
  analise->escrita_criacao += qtd;
}

//funcoes para verificacao do numero de comparacoes
void atualizaComparacoes_ordenacao(Analise *analise, int qtd){
  analise->comparacoes_ordenacao += qtd;
}

void atualizaComparacoes_criacao(Analise *analise, int qtd){
  analise->comparacoes_criacao += qtd;
}

//funcoes para atualizacao do tempo
void atualizaTempo_criacao(Analise *analise){
  //calcula o tempo o converte para milissegundos
  analise->tempo_criacao = ((double)(analise->fimTempo - analise->iniTempo) * 1000 / CLOCKS_PER_SEC);
}

void atualizaTempo_ordenacao(Analise *analise){
  //calculando e convertendo tempo total para milissegundos
  analise->tempo_ordenacao = ((double)(analise->fimTempo - analise->iniTempo) * 1000 / CLOCKS_PER_SEC);
}

//funcao para impressao dos dados 
void imprimirDados(Analise *analise){

  printf("Dados da execução para criação dos índices:\n");
  printf("⏰  - Tempo de execução: %.4lfms\n", analise->tempo_criacao);
  printf("📈  - Quantidade de comparação(ões): %d\n", analise->comparacoes_criacao);
  printf("📊  - Quantidade de transferencia(s) leitura: %d\n", analise->leitura_criacao); 
  printf("📊  - Quantidade de transferencia(s) escrita: %d\n", analise->escrita_criacao); 

  printf("Dados da execução da ordenacao:\n");
  printf("⏰  - Tempo de execução: %.4lfms\n", analise->tempo_ordenacao);
  printf("📈  - Quantidade de comparação(ões): %d\n", analise->comparacoes_ordenacao);
  printf("📊  - Quantidade de transferencia(s) leitura: %d\n", analise->leitura_ordenacao);
  printf("📊  - Quantidade de transferencia(s) escrita: %d\n", analise->escrita_ordenacao);
}