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

void binarioNaTela(char *nomeArquivoBinario) { /* Você não precisa entender o código dessa função. */

	/* Use essa função para comparação no run.codes. Lembre-se de ter fechado (fclose) o arquivo anteriormente.
	*  Ela vai abrir de novo para leitura e depois fechar (você não vai perder pontos por isso se usar ela). */

	unsigned long i, cs;
	unsigned char *mb;
	size_t fl;
	FILE *fs;
	if(nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb"))) {
		fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): não foi possível abrir o arquivo que me passou para leitura. Ele existe e você tá passando o nome certo? Você lembrou de fechar ele com fclose depois de usar?\n");
		return;
	}
	fseek(fs, 0, SEEK_END);
	fl = ftell(fs);
	fseek(fs, 0, SEEK_SET);
	mb = (unsigned char *) malloc(fl);
	fread(mb, 1, fl, fs);

	cs = 0;
	for(i = 0; i < fl; i++) {
		cs += (unsigned long) mb[i];
	}
	printf("%lf\n", (cs / (double) 100));
	free(mb);
	fclose(fs);
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

void escreverRegistro(FILE *arquivo, Dados *registro, Header *header) {
    int tamDestino = registro->tecDestino.tamString;
    int tamOrigem = registro->tecOrigem.tamString;

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
int main() {
    FILE *entrada = fopen("dados1.csv", "r");
    FILE *saida = fopen("saida.bin", "wb");
    Header header;
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
            escreverRegistro(saida, &registro, &header); // Escreve o registro no arquivo de saída
            imprimirRegistroNaTela(&registro); // Imprime o registro na tabela
        }
    }

    fclose(entrada);
    fclose(saida);
    
    //binarioNaTela("saida.bin");
    return 0;
}
