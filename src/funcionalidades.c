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

