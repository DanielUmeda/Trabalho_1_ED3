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
    char linha[TAM_REGISTRO];

    // Descarta a primeira linha
    if (fgets(linha, sizeof(linha), arquivo) == NULL) {
        // Se não foi possível ler a primeira linha, retorna
        return;
    }
    
    char *token;
    token = strtok(linha, ",");
        registro->removido = '0';
        // Lê o campo tecOrigem
        registro->tecOrigem.tamString = strlen(token);
        strcpy(registro->tecOrigem.nomeString, token);
                
        // Lê o campo grupo
        token = strtok(NULL, ",");
        sscanf(token, "%d", &registro->grupo);
        
        // Lê o campo pop
        token = strtok(NULL, ",");
        sscanf(token, "%d", &registro->pop);
        
        // Lê o campo tecDestino
        token = strtok(NULL, ",");
        registro->tecDestino.tamString = strlen(token);
        strcpy(registro->tecDestino.nomeString, token);
        // Lê o campo peso
        token = strtok(NULL, ",");
        sscanf(token, "%d", &registro->peso);
        
        
    
}
void imprimirRegistro(Dados *registro) {
  
    
    printf("| %.*s | %d | %d | %.*s | %d |\n",
           registro->tecOrigem.tamString,
           registro->tecOrigem.nomeString,
           registro->grupo,
           registro->pop,
           registro->tecDestino.tamString,
           registro->tecDestino.nomeString,
           registro->peso);
}


void escreverRegistro(FILE *arquivo, Dados *registro) {
    fwrite(&registro->removido, sizeof(char), 1, arquivo);
    fwrite(registro->tecOrigem.nomeString, sizeof(char), TAM_STRING_VARIAVEL, arquivo);
    fwrite(&registro->grupo, sizeof(int), 1, arquivo);
    fwrite(&registro->pop, sizeof(int), 1, arquivo);
    fwrite(registro->tecDestino.nomeString, sizeof(char), TAM_STRING_VARIAVEL, arquivo);
    fwrite(&registro->peso, sizeof(int), 1, arquivo);
}

int main() {
    FILE *entrada = fopen("tecnologia.csv", "r");
    FILE *saida = fopen("saida.bin", "wb");

    if (entrada == NULL || saida == NULL) {
        printf("Erro ao abrir os arquivos.\n");
        return 1;
    }

    // Leitura e escrita dos registros
    while (!feof(entrada)) {
        Dados registro;
        lerRegistro(entrada, &registro);
        if (registro.removido != '1') {
            escreverRegistro(saida, &registro); // Escreve o registro no arquivo de saída
            imprimirRegistro(&registro); // Imprime o registro na tabela
        }
    }

    fclose(entrada);
    fclose(saida);

    return 0;
}
