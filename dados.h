#ifndef DADOS_H
#define DADOS_H

#include "tecnologia.h"
#include <stdio.h>

#define TAM_CAMPOS_FIXOS 21
#define TAM_REGISTRO 76
#define TAM_CABECALHO 13
#define MAX_LIXO 196
#define MAX_M 4
#define TAM_CHAVE 100 // A definir ainda

typedef struct CampoVariavel
{
    int tamString;
    char nomeString[TAM_STRING_VARIAVEL];
} CampoVariavel;

typedef struct Dados
{
    char removido;
    int grupo;
    int pop;
    int peso;
    CampoVariavel tecOrigem;
    CampoVariavel tecDestino;
} Dados;

typedef struct Header
{
    char status;
    int proxRRN;
    int nroTecnologias;
    int nroParesTecnologias;
} Header;


void adicionarTecnologia(Tecnologia tecnologias[], int *numTecnologias, char *nomeTecnologias, Header *header);
void fecharArquivo(FILE *arquivo, Header *header);
void lerRegistro(FILE *arquivo, Dados *registro);
void inicializarHeader(Header *header);
void atualizarHeader(FILE *arquivo, Header *header);
void imprimirRegistrosNaTela(Dados *registro);
void preencherLixo(FILE *arquivo, Dados *registro, int tamRealReg);
void escreverRegistro(FILE *arquivo, Dados *registro, Header *header, Tecnologia tecTotal[], int *numTecTotal, Tecnologia tecPar[], int *numTecPar);
void imprimirTecnologiasUnicas(int numTecTotal, int numTecPar);
void lerSaida(FILE *saida, Dados *out);
void imprimirSaida(Dados *out);

#endif