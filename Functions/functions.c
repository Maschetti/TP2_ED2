#include "functions.h"

void imprimeArquivo(FILE* arquivo, int numeroItens) {
  Aluno auxiliar;
  for(int i = 0; i < numeroItens; i++) {
    fscanfAluno(arquivo, &auxiliar);
    printAluno(auxiliar);
  }
}

void parametrosParaInteiro(int numeroArgumentos, char **argumentos, int *metodo, int *quantidade, int *situacao, int *P) {
  if(numeroArgumentos >= 4 && numeroArgumentos < 6) { 
    *metodo = atoi(argumentos[1]);
    *quantidade = atoi(argumentos[2]);
    *situacao = atoi(argumentos[3]);
    *P = 0;

    if(numeroArgumentos == 5) {
      if(strcmp(argumentos[4], "-P") == 0) {
        *P = 1;
      }
    }
  }
  else {
    printf("ERROR: Numero de argumentos invalidos\n");
    printf("Entrada: pesquisa <metodo> <quantidade> <situacao> [-P]\n");
    printf("OBS: apenas o parametro [-P] e opcional\n");
  }
}

FILE* validaEntrada(int numeroArgumentos, int metodo, int quantidade, int situacao, int P) {
  FILE* arquivo = NULL;
  int valido = 1;

  if(metodo < 1 || metodo > 4) {
    printf("ERROR: metodo invalido\n");
    printf("OBS: metodo deve ser de 1 a 3\n\n");
    valido = 0;
  }

  if(quantidade < 1 || quantidade > 471705) {
    printf("ERROR: quantidade invalida\n");
    printf("OBS: o arquivo deve conter de 1 a 471.705 de registros\n\n");
    valido = 0;
  }

  if(situacao < 1 || situacao > 3) {
    printf("ERROR: situacao invalida\n");
    printf("OBS: a situacao do arquivo pode ser (1)ascendente (2)descendente (3)desordenado\n\n");
    valido = 0;
  }

  if(valido) {
    if(situacao == 1) {
      arquivo = fopen("ascendente.txt", "r");
    }
    else if(situacao == 2) {
      arquivo = fopen("descendente.txt", "r");
    }
    else {
      arquivo = fopen("PROVAO.txt", "r");
    }
  }

  if(P == 1) {
    imprimeArquivo(arquivo, quantidade);
  }

  return arquivo;
}

void printaBloco(FILE *arquivo) {
  int numeroItens;
  fread(&numeroItens, sizeof(int), 1, arquivo);
  Aluno aux;
  printf("%d\n", numeroItens);
  for(int i = 0; i < numeroItens; i ++) {
    fread(&aux, sizeof(Aluno), 1, arquivo);
    printAluno(aux);
  }
}

int comparaItem(Item x, Item y) {
  if(x.fim) return 1;
  if(y.fim) return 0;
  if(x.marcado == y.marcado) return (x.aluno.nota > y.aluno.nota);
  if(x.marcado) return 1;
  return 0;
}

void heapBE(BlocoEntrada *bloco, int posicaoPai, int numeroFitasEntrada) {
  Item auxiliar = bloco->itens[posicaoPai];
  int posicaoFilho = posicaoPai * 2 + 1;
  while(posicaoFilho < numeroFitasEntrada) {
    if(posicaoFilho < numeroFitasEntrada - 1) {
      if(!comparaItem(bloco->itens[posicaoFilho + 1], bloco->itens[posicaoFilho])) posicaoFilho++; 
    }
    if(!comparaItem(bloco->itens[posicaoFilho], auxiliar)) {
      bloco->itens[posicaoPai] = bloco->itens[posicaoFilho];
      posicaoPai = posicaoFilho;
      posicaoFilho = 2 * posicaoPai + 1;
    }
    else posicaoFilho = numeroFitasEntrada;
  }
  bloco->itens[posicaoPai] = auxiliar;
}

void heapBlocoEntrada(BlocoEntrada *bloco, int numeroFitasEntrada) {
  for(int posicaoPai = (numeroFitasEntrada - 1) / 2; posicaoPai >= 0; posicaoPai--) {
    heapBE(bloco, posicaoPai, numeroFitasEntrada);
  }
}

void reescreveContadorAlunos(FILE *arquivo, int contadorAlunos, long desloc) {
  fseek(arquivo, desloc, SEEK_SET);
  fwrite(&contadorAlunos, sizeof(int), 1, arquivo);
  fseek(arquivo, 0, SEEK_END);
}

Fita* iniciaFitas() {
  Fita *fitas = malloc(sizeof(Fita) * (2 * f));
  char nomeArquivo[20];
  FILE *arquivo = NULL;
  for(int i = 0; i < (2 * f); i++) {
    snprintf(nomeArquivo, 20, "Fitas/f_%d.bin", i + 1);
    arquivo = fopen(nomeArquivo, "w+b");
    iniciaFita(&fitas[i], arquivo);
  }
  return fitas;
}

void ponteirosInicioFitas(Fita *fitas) {
  for(int i = 0; i < 2 * f; i++) {
    fseek(fitas[i].arquivo, 0, SEEK_SET);
  }
}

void encheBlocoEntrada(FILE *arquivo, BlocoEntrada *bloco, int numeroFitasEntrada) {
  Aluno aluno;
  bloco->itens = malloc(sizeof(Item) * numeroFitasEntrada);
  for(int i = 0; i < numeroFitasEntrada; i++) {
    fscanfAluno(arquivo, &aluno);
    bloco->itens[i].aluno = aluno;
    bloco->itens[i].fim = 0;
    bloco->itens[i].marcado = 0;
  }
}

void substituicaoSelecao(FILE *arquivo, Fita *fitas, int numeroAlunos, int numeroFitasEntrada) {
  BlocoEntrada bloco;
  int contadorAlunos, alunosPrintados = 0, alunosNaoLidos = numeroAlunos, i = 0;
  long desloc;

  encheBlocoEntrada(arquivo, &bloco, numeroFitasEntrada);
  alunosNaoLidos -= numeroFitasEntrada;
  heapBlocoEntrada(&bloco, numeroFitasEntrada);

  while(alunosPrintados < numeroAlunos) {
    contadorAlunos = 0;
    desloc = ftell(fitas[i].arquivo);
    fwrite(&contadorAlunos, sizeof(int), 1, fitas[i].arquivo);
    
    while(!bloco.itens[0].fim && !bloco.itens[0].marcado) {
      fwriteAluno(bloco.itens[0].aluno, fitas[i].arquivo);
      bloco.ultimoInserido = bloco.itens[0].aluno.nota;
      alunosPrintados++;
      contadorAlunos++;

      if(alunosNaoLidos > 0) {
        fscanfAluno(arquivo, &bloco.itens[0].aluno);
        alunosNaoLidos--;

        if(bloco.itens[0].aluno.nota < bloco.ultimoInserido) bloco.itens[0].marcado = 1;
      }
      else bloco.itens[0].fim = 1;

      heapBE(&bloco, 0, numeroFitasEntrada);
    }

    if(contadorAlunos > 0) {
      fitas[i].numeroBlocos++;
      reescreveContadorAlunos(fitas[i].arquivo, contadorAlunos, desloc);
    }

    for(int i = 0; i < numeroFitasEntrada; i++) bloco.itens[i].marcado = 0;

    i = (i + 1) % numeroFitasEntrada;
  }
}