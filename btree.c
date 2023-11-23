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

void lerCabecalho(cabecalhoArvore *cabecalho, FILE *arquivoIndice){

    fread(&cabecalho->status, sizeof(char), 1, arquivoIndice);
    fread(&cabecalho->noRaiz, sizeof(int), 1, arquivoIndice);
    fread(&cabecalho->RRNproxNo, sizeof(int), 1, arquivoIndice);
    fread(&cabecalho->lixo, sizeof(char), 196, arquivoIndice);
    //Chegou no fim do cabeçalho

}
void lerNo( No *no, FILE *arquivoIndice){
    
    fread(&no->nroChavesNo, sizeof(int), 1, arquivoIndice);
    fread(&no->alturaNo, sizeof(int), 1, arquivoIndice);
    fread(&no->RRNdoNo, sizeof(int), 1, arquivoIndice);
    fread(&no->nroChavesNo, sizeof(int), 1, arquivoIndice);

    for (int i = 0; i < M; i++) {
        fread(&(no->P[i]), sizeof(int), 1, arquivoIndice);

        if (i < M-1) {
            fread(&(no->C[i]), sizeof(char), 55, arquivoIndice);
            fread(&(no->Pr[i]), sizeof(int), 1, arquivoIndice);
        }
    }
    for (int j = 0; j < no->nroChavesNo; j++)
    {
        for(int i = 0; i < 55; i++){
            if(no->C[j][i] == '&'){
                no->C[j][i] = '\0';
                break;
            }
        }
    }
    
}

int encontrarRRN(char *busca, int rrnDaRaiz, FILE *arquivoIndice){
    fseek(arquivoIndice, 0, SEEK_SET);
    int posicaoRaiz = (rrnDaRaiz +1)* 205;
    fseek(arquivoIndice, posicaoRaiz, SEEK_CUR);

    No *no;
        lerNo(&no, arquivoIndice);
        for (int i = 0; i < no->nroChavesNo; i++)
        {
            int resultado = strcmp(busca, no->C[i]);
            
            if(i == 3){
                return encontrarRRN(busca, no->P[i], arquivoIndice);  
            }
            if(resultado < 0){
                return encontrarRRN(busca, no->P[i], arquivoIndice);
            }
            else if(resultado == 0){
                //busca é maior que C[i]
                return no->Pr[i];
            }
        }
        return -1;
}