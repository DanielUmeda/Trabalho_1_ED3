#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "funcionalidades.h"
#include "dados.h"
#include "funcoesFornecidas.h"

void func1(FILE *entrada, FILE *saida){
    Header header;
    Tecnologia tecTotal[500];
    Tecnologia tecPar[500];


    int numTecTotal = 0;
    int numTecPar = 0;
    inicializarHeader(&header);
  

    if (entrada == NULL || saida == NULL) {
        printf("Falha no processamento do arquivo.");
        return;
    }

    fscanf(entrada, "%*[^\n]\n");
// Antes de iniciar a escrita de registros, verifique o estado inicial das variáveis
    fseek(saida, 13, SEEK_SET);
    while (!feof(entrada)) {
        Dados registro;
        lerRegistro(entrada, &registro);
        if (registro.removido == '0') {
           // Antes de iniciar a escrita de registros, verifique o estado inicial das variáveis
            escreverRegistro(saida, &registro, &header, tecTotal, &numTecTotal, tecPar, &numTecPar);
                //imprimirRegistrosNaTela(&registro);
        }
    }

    fclose(entrada);
    fclose(saida);
    //fecharArquivo(saida, &header);
    //header.status = '1';
    return;

}

void func2(FILE *saida){

        if (saida == NULL) {
        printf("Falha no processamento do arquivo.");
        return;
    }

    //pula o cabecalho do arquivo
    fseek(saida, 13, SEEK_SET);
    Dados out;
    if(!(fread(&out.removido, sizeof(char), 1, saida))){
        printf("Registro inexistente");
    }
    
    fseek(saida, 13, SEEK_SET);

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

void func3(FILE *entrada, char *campo, char *busca, Dados *total){
    
    if (entrada == NULL) {
        printf("Erro ao abrir os arquivos.\n");
        return;
    }
    
    Dados out;
    while(fread(&out.removido, sizeof(char), 1, entrada)){
        if (out.removido == '0')
        {
        fread(&out.grupo, sizeof(int), 1, entrada);
        fread(&out.pop, sizeof(int), 1, entrada);
        fread(&out.peso, sizeof(int), 1, entrada);

        fread(&out.tecOrigem.tamString, sizeof(int), 1, entrada);
        char buffer1[out.tecOrigem.tamString];
        fread(buffer1, sizeof(char), out.tecOrigem.tamString, entrada);
        strcpy(out.tecOrigem.nomeString, buffer1);


        fread(&out.tecDestino.tamString, sizeof(int), 1, entrada);
        char buffer[out.tecDestino.tamString];
        fread(buffer, sizeof(char), out.tecDestino.tamString, entrada);
        strcpy(out.tecDestino.nomeString, buffer);

        if (strcmp(campo, "nomeTecnologiaOrigem") == 0) {
            if (strcmp(busca, out.tecOrigem.nomeString) == 0) {
                    imprimirRegistrosNaTela(&out);
            }
        }   else if (strcmp(campo, "grupo") == 0) {
            int busca_int;
            if (sscanf(busca, "%d", &busca_int) == 1 && busca_int == out.grupo) {
                    imprimirRegistrosNaTela(&out);
            }
        }   else if (strcmp(campo, "popularidade") == 0) {
                int busca_int;
                if (sscanf(busca, "%d", &busca_int) == 1 && busca_int == out.pop) {
                    imprimirRegistrosNaTela(&out);
                 }
        }   else if (strcmp(campo, "nomeTecnologiaDestino") == 0) {
                if (strcmp(busca, out.tecDestino.nomeString) == 0) {
                    imprimirRegistrosNaTela(&out);
            }
        }   else if (strcmp(campo, "peso") == 0) {
                int busca_int;
                if (sscanf(busca, "%d", &busca_int) == 1 && busca_int == out.peso) {
                    imprimirRegistrosNaTela(&out);
            }
        }
        }
    }

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
    if(rrn>proxRRN){
        printf("Registro inexistente.");
    }
    fclose(saida);
}