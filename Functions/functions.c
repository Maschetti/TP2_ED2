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

int comparaItem(Item x, Item y) {
  if(x.aluno.nota == -1) return 1;
  if(y.aluno.nota == -1) return 0;
  if((x.aluno.nota > y.aluno.nota) && (x.marcado == y.marcado)) return 1;
  if(x.marcado) return 1;
  return 0;
}

void heapBE(BlocoEntrada *bloco, int posicaoPai) {
  Item auxiliar = bloco->itens[posicaoPai];
  int posicaoFilho = posicaoPai * 2 + 1;
  while(posicaoFilho < f) {
    if(posicaoFilho < f - 1) {
      if(!comparaItem(bloco->itens[posicaoFilho + 1], bloco->itens[posicaoFilho])) posicaoFilho++; 
    }
    if(!comparaItem(bloco->itens[posicaoFilho], auxiliar)) {
      bloco->itens[posicaoPai] = bloco->itens[posicaoFilho];
      posicaoPai = posicaoFilho;
      posicaoFilho = 2 * posicaoPai + 1;
    }
    else posicaoFilho = f;
  }
  bloco->itens[posicaoPai] = auxiliar;
}

void heapBlocoEntrada(BlocoEntrada *bloco) {
  for(int posicaoPai = (f - 1) / 2; posicaoPai >= 0; posicaoPai--) {
    heapBE(bloco, posicaoPai);
  }
}

int todosMarcados(BlocoEntrada bloco) {
  for(int i = 0; i < f; i++) {
    if(bloco.itens[i].marcado == 0) return 0;
  }
  return 1;
}

int todosNegativos(BlocoEntrada bloco) {
  for(int i = 0; i < f; i++) {
    if(bloco.itens[i].aluno.nota != -1) return 0;
  }
  return 1;
}

void reescreveContadorAlunos(FILE *arquivo, int contadorAlunos, long desloc) {
  // long desloc = (((sizeof(Aluno) + (sizeof(" ") * 4)) * contadorAlunos) + (sizeof("\n") * (contadorAlunos)) * -1);
  fseek(arquivo, desloc, SEEK_SET);
  fwrite(&contadorAlunos, sizeof(int), 1, arquivo);
  // fprintf(arquivo, "%d\n", contadorAlunos);
  fseek(arquivo, 0, SEEK_END);
}

Fita* iniciaFitas(int numeroFitas) {
  Fita *fitas = malloc(sizeof(Fita) * (numeroFitas));
  char nomeArquivo[16];
  FILE *arquivo = NULL;
  for(int i = 0; i < (numeroFitas); i++) {
    snprintf(nomeArquivo, 16, "Fitas/f_%d.bin", i + 1);
    arquivo = fopen(nomeArquivo, "w+b");
    iniciaFita(&fitas[i], arquivo);
  }
  return fitas;
}

void substituicaoSelecao(FILE *arquivo, Fita *fitas, int numeroAlunos) {
  BlocoEntrada bloco;
  Aluno aluno;
  Item item;
  long desloc;
  int contadorAlunos;
  zeraBlocoEntrada(&bloco);
  // preenche o bloco
  for(int i = 0; i < f; i++) {
    fscanfAluno(arquivo, &aluno);
    bloco.itens[i].aluno = aluno;
  }
  numeroAlunos -= f;

  //criacao de blocos
  while(numeroAlunos > 0) {
    for(int i = 0; i < f; i++) {
      contadorAlunos = 0;
      //salva a posicao para substituicao do contador
      desloc = ftell(fitas[i].arquivo);
      fwrite(&contadorAlunos, sizeof(int), 1, fitas[i].arquivo);
      while(!todosMarcados(bloco) && !todosNegativos(bloco)) {
        heapBlocoEntrada(&bloco);
        fwriteAluno(bloco.itens[0].aluno, fitas[i].arquivo);
        contadorAlunos++;
        bloco.ultimoInserido = bloco.itens[0].aluno.nota;

        //caso ja tanha sido lido todos os valores de entrada para de ler
        if(numeroAlunos != 0) {
          fscanfAluno(arquivo, &aluno);
          numeroAlunos--;

          item.aluno = aluno;
          if((item.aluno.nota < bloco.ultimoInserido)) {
            item.marcado = 1;
          }
          else zeraItem(&item);

          bloco.itens[0] = item;
        }
        // marca os itens caso esteja nos ultimos itens
        else {
          bloco.itens[0].aluno.nota = -1;
        }
      }
      if(contadorAlunos != 0) {
        fitas[i].numeroBlocos++;
        reescreveContadorAlunos(fitas[i].arquivo, contadorAlunos, desloc);
      }
      if(todosNegativos(bloco)) break;

      zeraBlocoEntrada(&bloco);
    }
  }
}