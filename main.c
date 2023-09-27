#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TAM_CAMPOS_FIXOS 21
#define TAM_REGISTRO 76
#define TAM_STRING_VARIAVEL (TAM_REGISTRO - TAM_CAMPOS_FIXOS)

typedef struct CampoVariavel{
    int tamString;
    char nomeString[TAM_STRING_VARIAVEL];
}CampoVariavel;

typedef struct Dados {
    char removido;
    int grupo;
    int pop;
    int peso;
    CampoVariavel tecOrigem;
    CampoVariavel tecDestino;
} Dados;   


void lerRegistro(FILE *arquivo, Dados *registro) {
    fread(&registro->removido, sizeof(char), 1, arquivo);
    fread(&registro->grupo, sizeof(int), 1, arquivo);
    fread(&registro->pop, sizeof(int), 1, arquivo);
    fread(&registro->peso, sizeof(int), 1, arquivo);
    fread(registro->tecOrigem.nomeString, sizeof(char), TAM_STRING_VARIAVEL, arquivo);
    fread(registro->tecDestino.nomeString, sizeof(char), TAM_STRING_VARIAVEL, arquivo);

}

void escreverRegistro(FILE *arquivo, Dados *registro) {
    fwrite(&registro->removido, sizeof(char), 1, arquivo);
    fwrite(&registro->grupo, sizeof(int), 1, arquivo);
    fwrite(&registro->pop, sizeof(int), 1, arquivo);
    fwrite(&registro->peso, sizeof(int), 1, arquivo);
    fwrite(registro->tecOrigem.nomeString, sizeof(char), TAM_STRING_VARIAVEL, arquivo);
    fwrite(registro->tecDestino.nomeString, sizeof(char), TAM_STRING_VARIAVEL, arquivo);
}

int main() {
    FILE *entrada = fopen("tecnologia.csv", "rb");
    FILE *saida = fopen("saida.bin", "wb");

    if (entrada == NULL || saida == NULL) {
        printf("Erro ao abrir os arquivos.\n");
        return 1;
    }

    // Cabeçalho (se houver)
    char status;
    fread(&status, sizeof(char), 1, entrada);
    fwrite(&status, sizeof(char), 1, saida);

    // Leitura e escrita dos registros
    while (!feof(entrada)) {
        Dados registro;
        lerRegistro(entrada, &registro);
        escreverRegistro(saida, &registro);
    }

    fclose(entrada);
    fclose(saida);

    
    return 0;
}