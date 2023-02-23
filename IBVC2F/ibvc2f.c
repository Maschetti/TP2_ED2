#include "ibvc2f.h"

void IBVC2F(FILE* arquivo, int numeroAlunos) {
  // inicia as fitas
  Fita *fitas = iniciaFitas();
  // coloca os valores de entrada na fita
  substituicaoSelecao(arquivo, fitas, numeroAlunos);
}