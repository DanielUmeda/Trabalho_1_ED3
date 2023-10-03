#include <stdio.h>
#include "../include/dados.h"

int main() {
    Header header;
    FILE *entrada = fopen("dados1.csv", "r");
    FILE *saida = fopen("saida.bin", "wb");
    Tecnologia tecTotal[500];
    Tecnologia tecPar[500];

    int numTecTotal = 0;
    int numTecPar = 0;
    inicializarHeader(&header);
  

    if (entrada == NULL || saida == NULL) {
        printf("Erro ao abrir os arquivos.\n");
        return 1;
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
    header.status = '1';
    fecharArquivo(saida, &header);
    return 0;
}
