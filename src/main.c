#include <stdio.h>
#include "../include/dados.h"

int main() {
    FILE *entrada = fopen("dados1.csv", "r");
    FILE *saida = fopen("saida.bin", "wb");
    Header header;
    Tecnologia tecOrigem[100];
    int numTecOrigem = 0;
    Tecnologia tecDestino[100];
    int numTecDestino = 0;

    if (entrada == NULL || saida == NULL) {
        printf("Erro ao abrir os arquivos.\n");
        return 1;
    }

    fscanf(entrada, "%*[^\n]\n");

    while (!feof(entrada)) {
        Dados registro;
        inicializarHeader(&header);
        lerRegistro(entrada, &registro);
        if (registro.removido != '1') {
            escreverRegistro(saida, &registro, &header, tecOrigem, &numTecOrigem, tecDestino, &numTecDestino);
            imprimirTecnologiasUnicas(tecOrigem, numTecOrigem, tecDestino, numTecDestino);
        }
    }

    fclose(entrada);
    fclose(saida);

    return 0;
}
