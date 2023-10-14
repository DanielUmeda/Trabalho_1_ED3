#include <stdio.h>
#include <string.h>
#include "../include/funcionalidades.h"
#include "../include/dados.h"


void func1(FILE *entrada, FILE *saida){
    Header header;
    Tecnologia tecTotal[500];
    Tecnologia tecPar[500];


    int numTecTotal = 0;
    int numTecPar = 0;
    inicializarHeader(&header);
  

    if (entrada == NULL || saida == NULL) {
        printf("Erro ao abrir os arquivos.\n");
        return;
    }

    fscanf(entrada, "%*[^\n]\n");

    while (!feof(entrada)) {
        Dados registro;
        lerRegistro(entrada, &registro);
        if (registro.removido != '1') {
            escreverRegistro(saida, &registro, &header, tecTotal, &numTecTotal, tecPar, &numTecPar);
        }
    }
            imprimirTecnologiasUnicas(numTecTotal, numTecPar);

    fclose(entrada);
    fecharArquivo(saida, &header);
    header.status = '1';
    return;

}

void func2(FILE *saida){

        if (saida == NULL) {
        printf("Erro ao abrir os arquivos.\n");
        return;
    }

    //pula o cabecalho do arquivo
    fseek(saida, 13, SEEK_SET);
    Dados out;

    while(fread(&out.removido, sizeof(char), 1, saida)){
        if (out.removido == '0')
        {
        fread(&out.grupo, sizeof(int), 1, saida);
        fread(&out.pop, sizeof(int), 1, saida);
        fread(&out.peso, sizeof(int), 1, saida);

        fread(&out.tecOrigem.tamString, sizeof(int), 1, saida);
        char buffer1[out.tecOrigem.tamString];
        fread(buffer1, sizeof(char), out.tecOrigem.tamString, saida);
        strcpy(out.tecOrigem.nomeString, buffer1);


        fread(&out.tecDestino.tamString, sizeof(int), 1, saida);
        char buffer[out.tecDestino.tamString];
        fread(buffer, sizeof(char), out.tecDestino.tamString, saida);
        strcpy(out.tecDestino.nomeString, buffer);
        imprimirRegistrosNaTela(&out);
        }     
    }
    fclose(saida);
}

void func4(FILE *saida, int rrn){

    if (saida == NULL) {
        printf("Erro ao abrir os arquivos.\n");
        return;
    }
    int proxRRN = 0;
    //pula o cabecalho do arquivo
    fseek(saida, 13, SEEK_SET);
    Dados out;
    while(fread(&out.removido, sizeof(char), 1, saida)){
        if (out.removido == '0')
        {
        
        fread(&out.grupo, sizeof(int), 1, saida);
        fread(&out.pop, sizeof(int), 1, saida);
        fread(&out.peso, sizeof(int), 1, saida);

        fread(&out.tecOrigem.tamString, sizeof(int), 1, saida);
        char buffer1[out.tecOrigem.tamString];
        fread(buffer1, sizeof(char), out.tecOrigem.tamString, saida);
        strcpy(out.tecOrigem.nomeString, buffer1);


        fread(&out.tecDestino.tamString, sizeof(int), 1, saida);
        char buffer[out.tecDestino.tamString];
        fread(buffer, sizeof(char), out.tecDestino.tamString, saida);
        strcpy(out.tecDestino.nomeString, buffer);

        if(proxRRN == rrn){
        imprimirRegistrosNaTela(&out);
        }
        proxRRN++;
        }     
    }
    fclose(saida);
}