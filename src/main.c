/*


JOAO MARCELO FERREIRA BATTAGLINI - 13835472
DANIEL UMEDA KUHN - 13676541



*/


#include <stdio.h>
#include "../include/dados.h"
#include "../include/funcionalidades.h"


int main() {
    
    int funcionalidade;
    char input[20];
    char output[20];
    char teste[20];
    int rrn = 0;
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
        scanf("%s", teste);

        FILE *out = fopen(teste, "rb+");
        
        func2(out);
        
        break; 
    case (4):
        //le o arquivo binario
        scanf("%s %d", teste, &rrn);

        FILE *file4 = fopen(teste, "rb+");
        
        func4(file4, rrn);
        
        break; 
    default:
        break;
    }
}