#include "btree.h"

No *criarNo()
{
    No *novoNo = (No *)malloc(sizeof(No));
    novoNo->nroChavesNo = 0;
    novoNo->alturaNo = 0;
    novoNo->RRNdoNo = -1;

    for (int i = 0; i < M; i++)
    {
        novoNo->P[i] = -1;  // Setando os ponteiros p/ nós filhos como nulos (ainda não há nenhum)
        novoNo->Pr[i] = -1; // Valor inicial da referência no arquivo de dados.
        for (int j = 0; j < M - 1; j++)
        {
            // inicialmente, o nó ainda não possui nenhuma chave associada
            novoNo->C[j][0] = '\0';
        }
    }
    return novoNo;
}

void lerCabecalho(cabecalhoArvore *cabecalho, FILE *arquivoIndice)
{

    fread(&cabecalho->status, sizeof(char), 1, arquivoIndice);
    fread(&cabecalho->noRaiz, sizeof(int), 1, arquivoIndice);
    fread(&cabecalho->RRNproxNo, sizeof(int), 1, arquivoIndice);
    fread(&cabecalho->lixo, sizeof(char), 196, arquivoIndice);
    // Chegou no fim do cabeçalho
}
void lerNo(No *no, FILE *arquivoIndice)
{

    fread(&no->nroChavesNo, sizeof(int), 1, arquivoIndice);
    fread(&no->alturaNo, sizeof(int), 1, arquivoIndice);
    fread(&no->RRNdoNo, sizeof(int), 1, arquivoIndice);

    for (int i = 0; i < M; i++)
    {
        fread(&(no->P[i]), sizeof(int), 1, arquivoIndice);

        if (i < M - 1)
        {
            fread(&(no->C[i]), sizeof(char), 55, arquivoIndice);
            fread(&(no->Pr[i]), sizeof(int), 1, arquivoIndice);
        }
    }
    for (int j = 0; j < 3; j++)
    {
        for (int i = 0; i < 55; i++)
        {
            if (no->C[j][i] == '$')
            {
                no->C[j][i] = '\0';
            }
        }
    }
}

int encontrarRRNRec(char *busca, int rrnAtual, FILE *arquivoIndice)
{
    fseek(arquivoIndice, (rrnAtual + 1) * 205, SEEK_SET);

    No no;
    lerNo(&no, arquivoIndice);

    for (int i = 0; i <= 3; i++)
    {
        int resultado = strcmp(busca, no.C[i]);

        if(i == 3){
            if (no.P[i] != -1)
            {
                return encontrarRRNRec(busca, no.P[i], arquivoIndice);
            }
            else{
                return -1;
            }
        }
        if (resultado < 0 || no.C[i][0] == '\0'){
            //busca é menor que c[i], logo deve estar à esquerda
            if (no.P[i] != -1)
            {
                return encontrarRRNRec(busca, no.P[i], arquivoIndice);
            }
            else{
                return -1;
            }
        } else if (resultado == 0)
        {
            //busca é igual à c[i], retornar a referência
            return no.Pr[i];
        }
    }
     if (no.P[no.nroChavesNo] != -1) {
        return encontrarRRNRec(busca, no.P[no.nroChavesNo], arquivoIndice);
    } else {
        // Se não houver último filho, a busca não foi encontrada
        return -1;
    }    
}

int encontrarRRN(char *busca, int rrnDaRaiz, FILE *arquivoIndice){
    return encontrarRRNRec(busca, rrnDaRaiz, arquivoIndice);
}