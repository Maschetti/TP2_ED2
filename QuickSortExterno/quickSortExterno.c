#include "quickSorteExterno.h"
#include "../Analises/analise.h"

void areaVazia(Area *area) {
 area->quantidade = 0;
}

void selectionSort(Area *area, Analise *analise) {
  int menor = 0;
  for(int i = 0; i < area->quantidade;i++){
    menor = i;
    for (int j = i + 1; j < area->quantidade;j++){
      atualizaComparacoes_ordenacao(analise, 1);
      if(area->alunos[menor].nota > area->alunos[j].nota) menor = j;
    }
    if (menor != i) { //se o numero seguinte e menor que o enterio e necessario uma troca
      Aluno aux;
      aux = area->alunos[menor];
      area->alunos[menor] = area->alunos[i];
      area->alunos[i] = aux;
    }
  }
}

int numeroCelulasOcupadas(Area *area) {
  return area->quantidade; //obetendo o numero de intens que esta na area
}

void insereItem(Aluno *ultimoLido, Area *area) {
  area->alunos[area->quantidade] = *ultimoLido; //colocando um intem no ultimo lugar do vector
  area->quantidade++; //aumentando a quantidade de intens presentes na area
}

void insereArea(Area *area, Aluno *ultimoLido, int *quantArea, Analise *analise) {
  //insere o ultimo lido de forma ordenada na area
  insereItem(ultimoLido, area);
  *quantArea = numeroCelulasOcupadas(area);
  selectionSort(area, analise);//metodo que ordena a area
}

void leInferior(FILE **ArqLi, Aluno *ultLido, int *li, int *ondeLer, Analise *analise) {
  fread(ultLido, sizeof(Aluno), 1, *ArqLi);
  atualizaLeitura_ordenacao(analise, 1);
  (*li)++;
  *ondeLer = 1;
}

void leSuperior(FILE **ArqLEs, Aluno *ultLido, int *ls, int *ondeLer, Analise *analise) {
  fseek(*ArqLEs, (*ls - 1) * sizeof (Aluno), SEEK_SET);
  atualizaLeitura_ordenacao(analise, 1);
  fread(ultLido, sizeof(Aluno), 1, *ArqLEs);
  (*ls)--;
  *ondeLer = 0;
}

void retiraUltimo(Area *area, Aluno *aluno) {
  *aluno = area->alunos[area->quantidade - 1];//tirando o ultimo
  area->quantidade--;//diminuindo a quantidades de intens que esta inseridos
}

void retiraMax(Area *area, Aluno *aluno, int *quantArea) {
  retiraUltimo(area, aluno);
  *quantArea = numeroCelulasOcupadas(area);
}

void retiraPrimeiro(Area *area, Aluno *aluno) {
  *aluno = area->alunos[0]; //retirando o primeiro
  for (int i = 1; i < area->quantidade;i++) { //reorganizando o vector pois o primeiro foi retirado
    area->alunos[i - 1] = area->alunos[i];
  }
  area->quantidade--;//diminuindo a quantidades de intens que esta inseridos
}

void retiraMin(Area *area, Aluno *aluno, int *quantArea) {
  retiraPrimeiro(area, aluno);
  *quantArea = numeroCelulasOcupadas(area);
}

void escreveMax(FILE **ArqLEs, Aluno aluno, int *es, Analise *analise){
  fseek(*ArqLEs, (*es - 1) * sizeof(Aluno), SEEK_SET);
  fwrite(&aluno, sizeof(Aluno), 1, *ArqLEs);
  atualizaEscrita_ordenacao(analise, 1);
  (*es)--;
}

void escreveMin(FILE **ArqEI, Aluno aluno, int *ei, Analise *analise){
  fwrite(&aluno, sizeof(Aluno), 1, *ArqEI);
  atualizaEscrita_ordenacao(analise, 1);
  (*ei)++;
}

void particao(FILE **ArqLi, FILE** ArqEi, FILE **ArqLEs, Area area, int esq, int dir, int *i, int *j, Analise *analise) {
  int ls = dir, es = dir, li = esq, ei = esq, quantArea = 0;
  double linf = INT_MIN, lsup = INT_MAX;
  int ondeLer = 1;
  Aluno ultimoLido, auxiliar;

  fseek(*ArqLi, (li - 1) * sizeof(Aluno),SEEK_SET);
  fseek(*ArqEi, (ei - 1) * sizeof(Aluno),SEEK_SET);
  *i = esq - 1;
  *j = dir + 1;

  while(ls >= li) {
    if(area.quantidade < f - 1) {
      if(ondeLer) leSuperior(ArqLEs, &ultimoLido, &ls, &ondeLer, analise);
      else leInferior(ArqLi, &ultimoLido, &li, &ondeLer, analise);

      insereArea(&area, &ultimoLido, &quantArea, analise);
      continue;
    }

    if(ls == es) leSuperior(ArqLEs, &ultimoLido, &ls, &ondeLer, analise);
    else if(li == ei) leInferior(ArqLi, &ultimoLido, &li, &ondeLer, analise);
    else if(ondeLer) leSuperior(ArqLEs, &ultimoLido, &ls, &ondeLer, analise);
    else leInferior(ArqLi, &ultimoLido, &li, &ondeLer, analise);

    atualizaComparacoes_ordenacao(analise, 1);
    if(ultimoLido.nota > lsup) {
      *j = es;
      escreveMax(ArqLEs, ultimoLido, &es, analise);
      continue;
    }
    atualizaComparacoes_ordenacao(analise, 1);
    if(ultimoLido.nota < linf) {
      *i = ei;
      escreveMin(ArqEi, ultimoLido, &ei, analise);
      continue;
    }

    insereArea(&area, &ultimoLido, &quantArea, analise);

    if (ei - esq < dir - es){
      retiraMin(&area, &auxiliar, &quantArea);
      escreveMin(ArqEi, auxiliar, &ei, analise);
      linf = auxiliar.nota;
    }
    else  {
      retiraMax(&area, &auxiliar, &quantArea);
      escreveMax(ArqLEs, auxiliar, &es, analise);
      lsup = auxiliar.nota;
    }
  }

  while(ei <= es) {
    retiraMin(&area, &auxiliar, &quantArea);
    escreveMin(ArqEi, auxiliar, &ei, analise);
  }
}

void quickSortExterno(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, int esq, int dir, Analise *analise) {
  int i, j;
  Area area;
  if(dir - esq < 1) return ;

  areaVazia(&area);
  particao(ArqLi, ArqEi, ArqLEs, area, esq, dir, &i, &j, analise);

  fflush(*ArqLi);
  fflush(*ArqEi);
  fflush(*ArqLEs);

  if (i - esq < dir - j){
    //ordena o subarquivo menor 
    quickSortExterno(ArqLi, ArqEi, ArqLEs, esq, i, analise);
    quickSortExterno(ArqLi, ArqEi, ArqLEs, j, dir, analise);
  }
  else {
    quickSortExterno(ArqLi, ArqEi, ArqLEs, j, dir, analise);
    quickSortExterno(ArqLi, ArqEi, ArqLEs, esq, i, analise);
  }
}

void quickSort(char* nomeArquivo, int numeroAlunos, Analise *analise) {
  FILE *ArgLEs; //arquivos que serao usados para leitura e escrita
  FILE *ArgLi;
  FILE *ArgEi;

  Aluno aluno;

  ArgLi = fopen(nomeArquivo, "r+b");
  ArgEi = fopen(nomeArquivo, "r+b");
  ArgLEs = fopen(nomeArquivo, "r+b");

  quickSortExterno(&ArgLi, &ArgEi, &ArgLEs, 1, numeroAlunos, analise);

  fflush(ArgLi);
  fclose(ArgEi);
  fclose(ArgLEs);

  fseek(ArgLi, 0, SEEK_SET);
  
  FILE *output = fopen("outBin.txt", "w");
  atualizaLeitura_ordenacao(analise, 1);
  while(fread(&aluno, sizeof(Aluno), 1, ArgLi)) {
    fprintAluno(aluno, output);
    atualizaLeitura_ordenacao(analise, 1);
  }

  fclose(output);
  fclose(ArgLi);
}