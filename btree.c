#include <stdlib.h>
#include "btree.h"

No* criarNo(){
    No *novoNo = (No*)malloc(sizeof(No));
    novoNo->nroChavesNo = 0;
    novoNo->alturaNo = 0;
    novoNo->RRNdoNo = -1;

    for (int i = 0; i < M; i++)
    {
        novoNo->P[i] = -1;        //Setando os ponteiros p/ nós filhos como nulos (ainda não há nenhum)
        novoNo->Pr[i] = -1;       //Valor inicial da referência no arquivo de dados.
        for (int j = 0; j < M-1; j++)
        {
            //inicialmente, o nó ainda não possui nenhuma chave associada
            novoNo->C[j][0] = '\0';
        }
    }
    return novoNo;
}

