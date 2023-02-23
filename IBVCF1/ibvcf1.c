#include "ibvcf1.h"

void IBVCF1(FILE* arquivo, int numeroAlunos) {
  // inicia as fitas
  Fita *fitas = iniciaFitas();
  // coloca os valores de entrada na fita
  substituicaoSelecao(arquivo, fitas, numeroAlunos);
}