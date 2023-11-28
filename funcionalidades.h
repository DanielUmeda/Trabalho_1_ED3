#ifndef FUNCONALIDADES_H
#define FUNCIONALIDADES_H

#include "dados.h"


void func1(FILE *entrada, FILE *saida);
void func2(FILE *saida);
void func3(FILE *entrada, char* campo, char* busca, Dados *total);
void func4(FILE *saida, int rrn);
void func5(FILE *entrada, FILE *saida);
int func6(FILE *arquivoDados, FILE *arquivoIndice, char* busca);
int func7(FILE *arquivoDados, FILE *arquivoIndice, int n);

#endif