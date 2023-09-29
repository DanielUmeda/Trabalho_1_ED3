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

typedef struct Header{
    char status;
    int proxRRN;
    int nroTecnologias;
    int nroParesTecnologias;
}Header;

typedef struct Tecnologia{
    char nome[TAM_STRING_VARIAVEL];
    int contagem;
}Tecnologia;

void adicionarTecnologia(Tecnologia tecnologias[], int *numTecnologias, char *nomeTecnologias){
    for (int i = 0; i < *numTecnologias; i++)
    {
        if(strcmp(tecnologias[i].nome, nomeTecnologias) == 0){
            tecnologias[i].contagem++;
            return;
        
    }
    }
    strcpy(tecnologias[*numTecnologias].nome, nomeTecnologias);
    tecnologias[*numTecnologias].contagem = 1;
    (*numTecnologias)++;
        
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

void inicializarHeader(Header *header){
    header->status = '0';
    header->proxRRN = 0;
    header->nroTecnologias = 0; 
    header->nroParesTecnologias = 0;
}

void atualizarHeader(FILE *arquivo, Header *header){
    fwrite(&header->status, sizeof(char), 1, arquivo);
    fwrite(&header->proxRRN, sizeof(int), 1, arquivo);
    fwrite(&header->nroTecnologias, sizeof(int), 1, arquivo);
    fwrite(&header->nroParesTecnologias, sizeof(int), 1, arquivo);
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

void escreverRegistro(FILE *arquivo, Dados *registro, Header *header,  Tecnologia tecOrigem[], int *numTecOrigem,Tecnologia tecDestino[], int *numTecDestino) {
    int tamDestino = registro->tecDestino.tamString;
    int tamOrigem = registro->tecOrigem.tamString;

    
    adicionarTecnologia(tecDestino, numTecDestino, registro->tecDestino.nomeString);
    adicionarTecnologia(tecOrigem, numTecOrigem, registro->tecOrigem.nomeString);
            
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

void imprimirTecnologiasUnicas(Tecnologia tecOrigem[], int numTecOrigem, Tecnologia tecDestino[], int numTecDestino) {
    printf("Tecnologias Origem Unicas:\n");
    for (int i = 0; i < numTecOrigem; i++) {
        printf("%s: %d\n", tecOrigem[i].nome, tecOrigem[i].contagem);
    }
    printf("Tecnologias Destino Unicas:\n");
    for (int i = 0; i < numTecDestino; i++) {
        printf("%s: %d\n", tecDestino[i].nome, tecDestino[i].contagem);
    }
}


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
    // Leitura e escrita dos registros
    while (!feof(entrada)) {
        Dados registro;
        inicializarHeader(&header);
        lerRegistro(entrada, &registro);
        if (registro.removido != '1') {
            escreverRegistro(saida, &registro, &header, tecOrigem, &numTecOrigem, tecDestino, &numTecDestino); // Escreve o registro no arquivo de saída
            //imprimirRegistroNaTela(&registro); // Imprime o registro na tabela
            imprimirTecnologiasUnicas(tecOrigem, numTecOrigem, tecDestino, numTecDestino);
        }
    }
    fclose(entrada);
    fclose(saida);
    //binarioNaTela("saida.bin");
    return 0;
}