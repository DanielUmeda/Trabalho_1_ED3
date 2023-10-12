#include <stdio.h>
#include "../include/dados.h"
#include "../include/funcionalidades.h"


int main() {
    
    int funcionalidade;
    char input[20];
    char output[20];


    scanf("%d", &funcionalidade);

    switch (funcionalidade)
    {
    case (1):
        //le o arquivo de entrada e saida (binario)
        scanf("%s %s", input, output);
        FILE *entrada = fopen(input, "r");
        FILE *saida = fopen(output, "wb+");
        func1(entrada, saida);

        break;
    case (2):
        //le o arquivo binario
        scanf("%s", output);

        FILE *out = fopen(output, "rb+");
        
        func2(out);
        
        break;
    default:
        break;
    }
}