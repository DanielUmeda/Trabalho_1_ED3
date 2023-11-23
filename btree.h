#ifndef BTREE_H
#define BTREE_H

#include <stdlib.h>

#define M 4

typedef struct No {
    int nroChavesNo; //Número atual de chaves no nó
    int alturaNo; //altura do nó na árvore
    int RRNdoNo; //rrn do nó no arq. de índices
    int P[M]; //Ponteiro p/ os nós filhos
    char C[M-1][55]; //Chaves de busca (string concatenada das tecnologias)
    int Pr[M-1]; //Referencia de cada chave p/ o arquivo de dados (rrn)
}No;


#endif
