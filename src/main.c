#include <stdio.h>
#include "../include/dados.h"

int main() {
    FILE *entrada = fopen("dados1.csv", "r");
    FILE *saida = fopen("saida.bin", "wb");
    Header header;
    Tecnologia tecTotal[500];
    int numTecTotal = 0;
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
            escreverRegistro(saida, &registro, &header, tecTotal, &numTecTotal);
        }
    }
            imprimirTecnologiasUnicas(numTecTotal);

    fclose(entrada);
    fclose(saida);

    return 0;
}
