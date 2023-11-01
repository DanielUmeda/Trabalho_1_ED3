#include "dados.h"
#include <stdio.h>
#include <string.h>

void adicionarTecnologia(Tecnologia tecnologias[], int *numTecnologias, char *nomeTecnologias, Header *header) {
    for (int i = 0; i < *numTecnologias; i++) {
        if (strcmp(tecnologias[i].nome, nomeTecnologias) == 0 && strcmp(tecnologias[i].nome, "NULO")!=0) {
            return;
        }
    }
    strcpy(tecnologias[*numTecnologias].nome, nomeTecnologias);
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

    char *token = linha;
    char *delimitadorCampos = ","; 
    registro->removido = '0';

    // Lê o campo tecOrigem
    char *endToken = strpbrk(token, delimitadorCampos); // Lê a string até encontrar uma vírgula 
    if (endToken != NULL) {
        if (endToken == token) {
            strcpy(registro->tecOrigem.nomeString, "NULO");
            //registro->tecOrigem.tamString = -1; // Campo nulo
        } else {
            *endToken = '\0';
            registro->tecOrigem.tamString = strlen(token);
            strcpy(registro->tecOrigem.nomeString, token);
        }
        token = endToken + 1;
    } else {
        registro->tecOrigem.tamString = 0; // Campo nulo
    }

    // Lê o campo grupo
    endToken = strpbrk(token, delimitadorCampos);
    if (endToken != NULL) {
        if (endToken == token) {
            registro->grupo = -1; // Campo nulo
        } else {
            *endToken = '\0';
            sscanf(token, "%d", &registro->grupo);
        }
        token = endToken + 1;
    } else {
        registro->grupo = -1; // Campo nulo
    }

    // Lê o campo pop
    endToken = strpbrk(token, delimitadorCampos);
    if (endToken != NULL) {
        if (endToken == token) {
            registro->pop = -1; // Campo nulo
        } else {
            *endToken = '\0';
            sscanf(token, "%d", &registro->pop);
        }
        token = endToken + 1;
    } else {
        registro->pop = -1; // Campo nulo
    }

    // Lê o campo tecDestino
    endToken = strpbrk(token, delimitadorCampos);
    if (endToken != NULL) {
        if (endToken == token) {
            strcpy(registro->tecDestino.nomeString, "NULO");
            //registro->tecDestino.tamString = -1; // Campo nulo
        } else {
            *endToken = '\0';
            registro->tecDestino.tamString = strlen(token);
            strcpy(registro->tecDestino.nomeString, token);
        }
        token = endToken + 1;
    } else {
        registro->tecDestino.tamString = 0; // Campo nulo
    }

    // Lê o campo peso
    char *delimitadorUltCampo = "\r\n";
    endToken = strpbrk(token, delimitadorUltCampo);
    if (endToken != NULL) {
        if (endToken == token) {
            registro->peso = -1; // Campo nulo
        } else {
            *endToken = '\0';
            sscanf(token, "%d", &registro->peso);
        }
        token = endToken + 1;
    } else {
        registro->peso = -1; // Campo nulo
    }
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

void imprimirRegistrosNaTela(Dados *registro) {

    //a funcao verifica os 2 possiveis casos para cada campo: nulo/nao nulo

    if(registro->tecOrigem.tamString == 0){
        printf("NULO, ");
    }
    else{
        printf("%.*s, ",
        registro->tecOrigem.tamString,
        registro->tecOrigem.nomeString
        );
    }
   
    
    if(registro->grupo == -1){
        printf("NULO, ");
    }
    else{
        printf("%d, ", registro->grupo);
    }

    if(registro->pop == -1){
        printf("NULO, ");
    }
    else{
        printf("%d, ", registro->pop);
    }
    if(registro->tecDestino.tamString == 0){
        printf("NULO, ");
    }
    else{
        printf("%.*s, ",
        registro->tecDestino.tamString,
        registro->tecDestino.nomeString
        );
    }

    if(registro->peso == -1){
        printf("NULO\n");
    }
    else{
        printf("%d\n", registro->peso);
    }

}

void preencherLixo(FILE *arquivo, Dados *registro, int tamRealRegistro) {

    int tamPreencher = TAM_REGISTRO - tamRealRegistro;
    //enquanto i for menor que o tamanho não preenchido do registro, insere-se 1 '$'
    for (int i = 0; i < tamPreencher; i++) {
        fwrite("$", sizeof(char), 1, arquivo);
    }
}

void escreverRegistro(FILE *arquivo, Dados *registro, Header *header, Tecnologia tecTotal[], int *numTecTotal, Tecnologia tecPar[], int *numTecPar) {
    
    
    int tamDestino = registro->tecDestino.tamString;
    int tamOrigem = registro->tecOrigem.tamString;
    int tamanhoRealRegistro = TAM_CAMPOS_FIXOS + sizeof(int) * 2 + tamOrigem + tamDestino;

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
    preencherLixo(arquivo, registro, tamanhoRealRegistro);
}


