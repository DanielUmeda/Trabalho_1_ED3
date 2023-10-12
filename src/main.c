#include <stdio.h>
#include "../include/dados.h"
#include "../include/funcionalidades.h"


int main() {
    
    int funcionalidade;
    char input[20];
    char output[20];


    scanf("%d %s %s", &funcionalidade, input, output);

    switch (funcionalidade)
    {
    case (1):

        FILE *entrada = fopen(input, "r");
        FILE *saida = fopen(output, "wb+");
        func1(entrada, saida);

        break;
    
    default:
        break;
    }
}