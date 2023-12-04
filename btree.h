#ifndef BTREE_H
#define BTREE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dados.h"


#define M 4
#define TAMANHO_REGISTRO 205

typedef struct No {
    int nroChavesNo; //Número atual de chaves no nó
    int alturaNo; //altura do nó na árvore
    int RRNdoNo; //rrn do nó no arq. de índices
    int P[M]; //Ponteiro p/ os nós filhos
    char C[M-1][55]; //Chaves de busca (string concatenada das tecnologias)
    int Pr[M-1]; //Referencia de cada chave p/ o arquivo de dados (rrn)
    int *prPromovido;
    char **auxPromovido;
}No;

typedef struct cabecalhoArvore{
    char status;
    int noRaiz;
    int RRNproxNo;
    char lixo[196];
}cabecalhoArvore;

No *criarNo();
void lerCabecalho(cabecalhoArvore *cabecalhoArvore, FILE *arquivoIndice);
void lerNo(No *no, FILE *arquivoIndice);
int encontrarRRNRec(char *busca, int rrnDaRaiz, FILE *arquivoIndice);
int encontrarRRN(char *busca, int rrnDaRaiz, FILE *arquivoIndice);
static int estaDisponivel(No *no);
static void insereNo(No *no, char *aux, int RRNReferencia, int lugar);
static void inserirNoLugar(No *no, char *aux, int RRNReferencia, int lugar);
static void mudaPonteiro(No *dadoNo, No *novoDireito, int lugar);
static void mudaPonteiroSplit(No *noEsquerdo, No *inferiorDireito, No *superiorDireito, int *pointers, int lugar);
char **promoverVetor(No *no, char **vetor, char *aux);
int ondeInserir(No *no, char *aux);
void atualizarArquivo(FILE *arquivoIndice, No *no, int RRNLugar);
int *prVetor(No *noFilho, int *prPromovido, char *aux, int RRNReferencia);
static No *dividirNo(FILE *arquivoIndice, No *noFilho, No *novoDireito, char *aux, int RRNReferencia);
int *promoverPonteiros(No *no, int ponteiros[]);
int eRaiz(FILE *arquivoIndice, No *raiz, int *NoMaisAlto);
No *lerRegistroIndice(FILE *arquivoIndice, No *prNo, int RRN);
int alturaArvore(FILE *arquivoIndice, No *no);
No *resgatarRaiz(FILE *arquivoIndice, No *raiz);
No *splitIntermediario(FILE *arquivoIndice, No *raiz, No *promovidoNoDireito, char **promovido, int *prPromovido, char *aux, int RRNReferencia, int *RRNNoINdice);
void atualizaHeader(FILE *arquivoIndice, char status, int RRNNoRaiz, int *RRNNoINdice);
No *insereLocal(FILE *arquivoIndice, No *raiz, char **promovido, char *aux, int RRNReferencia, int *RRNNoINdice, int *NoMaisAlto);
void novaRaizSplit(FILE *arquivoIndice, No *raiz, No *promovidoNoDireito, char **promovido, int *prPromovido, char *aux, int RRNReferencia, int *RRNNoINdice);
No *inserirStringIndice(FILE *arquivoIndice, int no_inIndex, char *aux, int *NoMaisAlto, int *RRNNoINdice, int RRNReferencia);
void inserePrimeiraRaiz(FILE *arquivoIndice);
void insereNoIndice(FILE *arquivoIndice, Dados *dados, int *NoMaisAlto, int *RRNNoINdice, int RRNReferencia);


#endif
