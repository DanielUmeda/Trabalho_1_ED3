#include "../include/dados.h"
#include <stdio.h>
#include <string.h>

void adicionarTecnologia(Tecnologia tecnologias[], int *numTecnologias, char *nomeTecnologias, Header *header) {
    for (int i = 0; i < *numTecnologias; i++) {
        if (strcmp(tecnologias[i].nome, nomeTecnologias) == 0) {
            return;
        }
    }
    strcpy(tecnologias[*numTecnologias].nome, nomeTecnologias);
    tecnologias[*numTecnologias].contagem = 1;
    (*numTecnologias)++;

}
void fecharArquivo(FILE *arquivo, Header *header){
    header->status = '1';
    atualizarHeader(arquivo, header);
    fclose(arquivo);
}

void lerRegistro(FILE *arquivo, Dados *registro) {
    
    char linha[TAM_REGISTRO];

    if (fgets(linha, sizeof(linha), arquivo) == NULL) {
        // Se não foi possível ler a primeira linha, retorna
        return;
    }
    
    char *token;
    token = strtok(linha, ",");
    registro->removido = '0';
    // Lê o campo tecOrigem
    if(token != NULL){
        registro->tecOrigem.tamString = strlen(token);
        strcpy(registro->tecOrigem.nomeString, token);
    }
    else{
        registro->tecOrigem.tamString = 0;
    }
    // Lê o campo grupo
    token = strtok(NULL, ",");
    sscanf(token, "%d", &registro->grupo);
    
    // Lê o campo pop
    token = strtok(NULL, ",");
    sscanf(token, "%d", &registro->pop);
    
    // Lê o campo tecDestino
    token = strtok(NULL, ",");
    if(token != NULL){
        registro->tecDestino.tamString = strlen(token);
        strcpy(registro->tecDestino.nomeString, token);
    }
    else{
        registro->tecDestino.tamString = 0;
    }

    // Lê o campo peso
    token = strtok(NULL, ",");
    sscanf(token, "%d", &registro->peso);

}

void inicializarHeader(Header *header) {
    header->status = '0';
    header->proxRRN = 0;
    header->nroTecnologias = 0; 
    header->nroParesTecnologias = 0;
}

void atualizarHeader(FILE *arquivo, Header *header) {
   long posicaoAtual = ftell(arquivo);

    // Move o ponteiro para o início do arquivo
    fseek(arquivo, 0, SEEK_SET);

    // Escreve os dados do cabeçalho
    fwrite(&header->status, sizeof(char), 1, arquivo);
    fwrite(&header->proxRRN, sizeof(int), 1, arquivo);
    fwrite(&header->nroTecnologias, sizeof(int), 1, arquivo);
    fwrite(&header->nroParesTecnologias, sizeof(int), 1, arquivo);

    // Move o ponteiro de volta para a posição original
    fseek(arquivo, posicaoAtual, SEEK_SET);
    
}

void imprimirRegistroNaTela(Dados *registro) {
      printf("| %.*s | %d | %d | %.*s | %d |\n",
        registro->tecOrigem.tamString,
        registro->tecOrigem.nomeString,
        registro->grupo,
        registro->pop,
        registro->tecDestino.tamString,
        registro->tecDestino.nomeString,
        registro->peso);
}

void preencherLixo(FILE *arquivo, Dados *registro) {
    int tamReal = TAM_REGISTRO - registro->tecDestino.tamString - registro->tecOrigem.tamString - TAM_CAMPOS_FIXOS;

    //enquanto i for menor que o tamanho não preenchido do registro, insere-se 1 '$'
    for (int i = 0; i < tamReal; i++) {
        fwrite("$", sizeof(char), 1, arquivo);
    }
}

void escreverRegistro(FILE *arquivo, Dados *registro, Header *header, Tecnologia tecTotal[], int *numTecTotal, Tecnologia tecPar[], int *numTecPar) {
    int tamDestino = registro->tecDestino.tamString;
    int tamOrigem = registro->tecOrigem.tamString;

    char strAux[100];
    strcpy(strAux, registro->tecOrigem.nomeString);
    strcat(strAux, registro->tecDestino.nomeString);

    adicionarTecnologia(tecTotal, numTecTotal, registro->tecDestino.nomeString, header);
    adicionarTecnologia(tecTotal, numTecTotal, registro->tecOrigem.nomeString, header);
    adicionarTecnologia(tecPar, numTecPar, strAux, header);
    
    header->proxRRN += 1;
    header->nroTecnologias = *numTecTotal;
    header->nroParesTecnologias = *numTecPar;
    atualizarHeader(arquivo, header);
    fwrite(&registro->removido, sizeof(char), 1, arquivo);
    fwrite(&registro->grupo, sizeof(int), 1, arquivo);
    fwrite(&registro->pop, sizeof(int), 1, arquivo);
    fwrite(&registro->peso, sizeof(int), 1, arquivo);
    fwrite(&registro->tecOrigem.tamString, sizeof(int), 1, arquivo);
    fwrite(registro->tecOrigem.nomeString, sizeof(char), tamOrigem, arquivo);
    fwrite(&registro->tecDestino.tamString, sizeof(int), 1, arquivo);
    fwrite(registro->tecDestino.nomeString, sizeof(char), tamDestino, arquivo);

    //Com o registro todo preenchido, devemos preencher o restante com $, até que os 76 bytes sejam utilizados
    preencherLixo(arquivo, registro);
}

void imprimirTecnologiasUnicas(int numTecTotal, int numTecPar) {

    printf("Qtde total de tecnologias diferentes: %d",  numTecTotal);
    printf("Qtde total de tecnologias diferentes: %d",  numTecPar);
}