/*
Daniel Umeda Kuhn - 13676541
João Marcelo Ferreira Battaglini - 13835472


*/


#include <stdio.h>
#include "dados.h"
#include "funcionalidades.h"
#include "funcoesFornecidas.h"
#include <stdlib.h>


int main() {

    int funcionalidade;
    char input[20];
    char output[20];
    char arquivo[20];
    char arquivo1[20];
    char arquivo2[20];
    int rrn = 0;
    int n = 0;
    Header header;
    scanf("%d", &funcionalidade);

    switch (funcionalidade)
    {
        case (1):
            //le o arquivo de entrada e saida (binario)
            scanf("%s %s", input, output);
            FILE *in1 = fopen(input, "r");

            fseek(in1, 0, SEEK_SET);
            fread(&header.status, sizeof(char), 1, in1);
            if (in1 == NULL || header.status == '0') {
                printf("Falha no processamento do arquivo.");
                break;
            }

            FILE *out1 = fopen(output, "wb+");
            func1(in1, out1);
            binarioNaTela(output);

            break;
        case (2):
            //le o arquivo binario
            scanf("%s", arquivo);

            FILE *file2 = fopen(arquivo, "rb");

            fseek(file2, 0, SEEK_SET);
            fread(&header.status, sizeof(char), 1, file2);
            if (file2 == NULL || header.status == '0') {
                printf("Falha no processamento do arquivo.");
                break;
            }

            func2(file2);

            break;
        case (3):
            scanf("%s%d", arquivo1, &n);

            char (*campo)[25] = malloc(n * sizeof(char[25]));
            char (*busca)[25] = malloc(n * sizeof(char[25]));

            FILE *file3 = fopen(arquivo1, "rb+");
            Dados total;

            

            fseek(file3, 0, SEEK_SET);
            fread(&header.status, sizeof(char), 1, file3);
            if (file3 == NULL || header.status == '0') {
                printf("Falha no processamento do arquivo.");
                break;
            }


            for (int i = 0; i < n; i++) {
                fseek(file3, 13, SEEK_SET);
                scanf("%s", campo[i]);
                scan_quote_string(busca[i]);
                func3(file3, campo[i], busca[i], &total);
            }
            break;

        case (4):
            //le o arquivo binario
            scanf("%s%d", arquivo2, &rrn);
            /*if(rrn > 490){
                printf("Registro inexistente.");
                break;
            }*/
            FILE *file4 = fopen(arquivo2, "rb+");

            fseek(file4, 0, SEEK_SET);
            fread(&header.status, sizeof(char), 1, file4);
            if (file4 == NULL || header.status == '0') {
                printf("Falha no processamento do arquivo.");
                break;
            }


            func4(file4, rrn);

            break;
        default:
            break;
    }
}
