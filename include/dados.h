#ifndef DADOS_H
#define DADOS_H

#include "tecnologia.h"
#include <stdio.h>

#define TAM_CAMPOS_FIXOS 21
#define TAM_REGISTRO 76

typedef struct CampoVariavel {
    int tamString;
    char nomeString[TAM_STRING_VARIAVEL];
} CampoVariavel;

typedef struct Dados {
    char removido;
    int grupo;
    int pop;
    int peso;
    CampoVariavel tecOrigem;
    CampoVariavel tecDestino;
} Dados;

typedef struct Header {
    char status;
    int proxRRN;
    int nroTecnologias;
    int nroParesTecnologias;
} Header;

void adicionarTecnologia(Tecnologia tecnologias[], int *numTecnologias, char *nomeTecnologias);
void lerRegistro(FILE *arquivo, Dados *registro);
void inicializarHeader(Header *header);
void atualizarHeader(FILE *arquivo, Header *header);
void imprimirRegistroNaTela(Dados *registro);
void preencherLixo(FILE *arquivo, Dados *registro);
void escreverRegistro(FILE *arquivo, Dados *registro, Header *header, Tecnologia tecOrigem[], int *numTecOrigem, Tecnologia tecDestino[], int *numTecDestino);
void imprimirTecnologiasUnicas(Tecnologia tecOrigem[], int numTecOrigem, Tecnologia tecDestino[], int numTecDestino);

#endif
