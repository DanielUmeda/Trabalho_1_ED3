#include "indices.h"

int atualizarIndices(Dados *registro, FILE *arquivoDados, FILE *arquivoIndices)
{
    char naoRemovido ='0';
    
    registro->tecOrigem.tamString = strlen(registro->tecOrigem.nomeString);
    registro->tecDestino.tamString = strlen(registro->tecDestino.nomeString);

    char stringConcatenada[55];
    strcpy(stringConcatenada, registro->tecOrigem.nomeString);
    strcat(stringConcatenada, registro->tecDestino.nomeString);

    cabecalhoArvore cabecalho;

    lerCabecalho(&cabecalho, arquivoIndices);
    if (cabecalho.status == '0')
    {
        printf("Falha no processamento do arquivo.\n");
        return -1;
    }
    //Escrevendo no arquivo de dados
    fseek(arquivoDados, 0, SEEK_END);
    fwrite(&naoRemovido, sizeof(char), 1, arquivoDados);
    fwrite(&registro->grupo, sizeof(int), 1, arquivoDados);
    fwrite(&registro->pop, sizeof(int), 1, arquivoDados);
    fwrite(&registro->peso, sizeof(int), 1, arquivoDados);

    fwrite(&registro->tecOrigem.tamString, sizeof(int), 1, arquivoDados);
    fwrite(registro->tecOrigem.nomeString, sizeof(char), registro->tecOrigem.tamString, arquivoDados);

    fwrite(&registro->tecDestino.tamString, sizeof(int), 1, arquivoDados);
    fwrite(registro->tecDestino.nomeString, sizeof(char), registro->tecDestino.tamString, arquivoDados);
    
    int tamRegistro = registro->tecDestino.tamString + registro->tecOrigem.tamString + 5 * sizeof(int) + 1 * sizeof(char);

    preencherLixo(arquivoDados, tamRegistro);



}