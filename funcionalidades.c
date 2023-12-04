#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "funcionalidades.h"
#include "dados.h"
#include "funcoesFornecidas.h"
#include "btree.h"
#include "indices.h"

void func1(FILE *entrada, FILE *saida){
    Header header;
    Tecnologia tecTotal[500];
    Tecnologia tecPar[500];

    int numTecTotal = 0;
    int numTecPar = 0;
    inicializarHeader(&header);

    if (entrada == NULL || saida == NULL)
    {
        printf("Falha no processamento do arquivo.");
        return;
    }

    fscanf(entrada, "%*[^\n]\n");
    // Antes de iniciar a escrita de registros, verifique o estado inicial das variáveis
    fseek(saida, 13, SEEK_SET);
    while (!feof(entrada))
    {
        Dados registro;
        lerRegistro(entrada, &registro);
        if (registro.removido == '0')
        {
            // Antes de iniciar a escrita de registros, verifique o estado inicial das variáveis
            escreverRegistro(saida, &registro, &header, tecTotal, &numTecTotal, tecPar, &numTecPar);
            // imprimirRegistrosNaTela(&registro);
        }
    }

    fclose(entrada);
    fclose(saida);
    // fecharArquivo(saida, &header);
    // header.status = '1';
    return;
}

void func2(FILE *saida)
{

    // pula o cabecalho do arquivo
    fseek(saida, 13, SEEK_SET);
    Dados out;
    if (!(fread(&out.removido, sizeof(char), 1, saida)))
        while (fread(&out.removido, sizeof(char), 1, saida))

        {
            printf("Registro inexistente");
        }
    int aux = 0;

    fseek(saida, 13, SEEK_SET);

    while (fread(&out.removido, sizeof(char), 1, saida))
    {
        if (out.removido == '0')
        {
            aux = 1;
            fread(&out.grupo, sizeof(int), 1, saida);
            fread(&out.pop, sizeof(int), 1, saida);
            fread(&out.peso, sizeof(int), 1, saida);

            fread(&out.tecOrigem.tamString, sizeof(int), 1, saida);
            char buffer1[(out.tecOrigem).tamString];
            fread(buffer1, sizeof(char), out.tecOrigem.tamString, saida);
            strcpy(out.tecOrigem.nomeString, buffer1);

            fread(&out.tecDestino.tamString, sizeof(int), 1, saida);
            char buffer[out.tecDestino.tamString];
            fread(buffer, sizeof(char), out.tecDestino.tamString, saida);
            strcpy(out.tecDestino.nomeString, buffer);
            imprimirRegistrosNaTela(&out);
        }
        else if (out.removido == '1')
        {
            fseek(saida, 75, SEEK_CUR);
        }
    }
    if (aux == 0)
    {
        printf("Registro inexistente.\n");
    }

    fclose(saida);
}

void func3(FILE *entrada, char *campo, char *busca, Dados *total)
{
    Header header;
    fseek(entrada, 0, SEEK_SET);
    fread(&header.status, sizeof(char), 1, entrada);
    if (entrada == NULL || header.status == '0')
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    int encontrado = 0;
    Dados out;
    while (fread(&out.removido, sizeof(char), 1, entrada))
    {

        if (out.removido == '0')
        {
            fread(&out.grupo, sizeof(int), 1, entrada);
            fread(&out.pop, sizeof(int), 1, entrada);
            fread(&out.peso, sizeof(int), 1, entrada);

            fread(&out.tecOrigem.tamString, sizeof(int), 1, entrada);
            char buffer1[out.tecOrigem.tamString];
            fread(buffer1, sizeof(char), out.tecOrigem.tamString, entrada);
            strcpy(out.tecOrigem.nomeString, buffer1);

            fread(&out.tecDestino.tamString, sizeof(int), 1, entrada);
            char buffer[out.tecDestino.tamString];
            fread(buffer, sizeof(char), out.tecDestino.tamString, entrada);
            strcpy(out.tecDestino.nomeString, buffer);

            if (strcmp(campo, "nomeTecnologiaOrigem") == 0)
            {
                if (out.tecOrigem.tamString == strlen(busca) && strncmp(busca, out.tecOrigem.nomeString, out.tecOrigem.tamString) == 0)
                {
                    imprimirRegistrosNaTela(&out);
                    encontrado = 1;
                }
            }
            else if (strcmp(campo, "grupo") == 0)
            {
                int busca_int;
                if (sscanf(busca, "%d", &busca_int) == 1 && busca_int == out.grupo)
                {
                    imprimirRegistrosNaTela(&out);
                    encontrado = 1;
                }
            }
            else if (strcmp(campo, "popularidade") == 0)
            {
                int busca_int;
                if (sscanf(busca, "%d", &busca_int) == 1 && busca_int == out.pop)
                {
                    imprimirRegistrosNaTela(&out);
                    encontrado = 1;
                }
            }
            else if (strcmp(campo, "nomeTecnologiaDestino") == 0)
            {
                if (out.tecDestino.tamString == strlen(busca) && strncmp(busca, out.tecDestino.nomeString, out.tecDestino.tamString) == 0)
                {
                    imprimirRegistrosNaTela(&out);
                    encontrado = 1;
                }
            }
            else if (strcmp(campo, "peso") == 0)
            {
                int busca_int;
                if (sscanf(busca, "%d", &busca_int) == 1 && busca_int == out.peso)
                {
                    imprimirRegistrosNaTela(&out);
                    encontrado = 1;
                }
            }
        }
        else if (out.removido == '1')
        {
            fseek(entrada, 75, SEEK_CUR);
        }
    }
    if (!encontrado)
    {
        printf("Registro inexistente.\n");
    }
}

void func4(FILE *saida, int rrn)
{

    int posicao = 76 * rrn;
    int encontrado = 0;
    // pula o cabecalho do arquivo
    fseek(saida, 13 + posicao, SEEK_SET);
    Dados out;
    fread(&out.removido, sizeof(char), 1, saida);

    if (out.removido == '0')
    {
        fread(&out.grupo, sizeof(int), 1, saida);
        fread(&out.pop, sizeof(int), 1, saida);
        fread(&out.peso, sizeof(int), 1, saida);

        fread(&out.tecOrigem.tamString, sizeof(int), 1, saida);
        char buffer1[out.tecOrigem.tamString];
        fread(buffer1, sizeof(char), out.tecOrigem.tamString, saida);
        strcpy(out.tecOrigem.nomeString, buffer1);

        fread(&out.tecDestino.tamString, sizeof(int), 1, saida);
        char buffer[out.tecDestino.tamString];
        fread(buffer, sizeof(char), out.tecDestino.tamString, saida);
        strcpy(out.tecDestino.nomeString, buffer);
        encontrado = 1;
    }

    if (encontrado == 1)
    {
        imprimirRegistrosNaTela(&out);
    }
    else
    {
        printf("Registro inexistente.\n");
    }
}

void func5(FILE *entrada, FILE *saida)
{

    /*
     *   Inicialização bem semelhante as outras funcionalidades
     *   Abre o arquivo binário, lê o cabeçario para posicionar devidamente a cabeça leitora para o primeiro RRN
    */
    if (entrada == NULL || saida == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return; // Caso o bin não seja encontrado ou o bin_index não criado
    }

    //  inicializa o cabeçalho
    cabecalhoArvore cabecalho;
    lerCabecalho(&cabecalho, entrada);
    if (cabecalho.status != '1')
    {
        printf("Falha no processamento do arquivo.\n");
        fclose(entrada);
        return;
    }

    //  inicializa o cabeçalho do arquivo de indice

    inicializarHeader(saida);
    inserePrimeiraRaiz(saida);

    Dados dados;
    int encontrado = 0;                  // para testar registro inexistente
    int NoMaisAlto = 0;                 // Valor da atual altura maxima da arvore
    int RRNNoINdice = 0;                // Contador para os nós inseridos
    int RRNReferencia = 0;                // Contador para os nós inseridos
    fseek(entrada, TAM_CABECALHO, SEEK_SET); //  Para pular o cabeçalho
    int counter = 0;
    // le tudo do registro desejado a seguir
    while (fread(&dados.removido, sizeof(char), 1, entrada))
    {

        if (dados.removido == '0')
        {
            counter++;
            // printf("inter:%d", counter);
            lerRegistro(entrada, &dados);
            // printa_registro(&dados); //  Utiliza a função já previamente criada na funcionalidade 3 para printar n tela o devido registro
            insereNoIndice(saida, &dados, &NoMaisAlto, &RRNNoINdice, RRNReferencia);
            No *raiz = criarNo();
            raiz = resgatarRaiz(saida, raiz);
            RRNReferencia++; //  Contador do registros de leitura do arquivo arquivoário
            encontrado = 1; //  Caso encontre, pelo menos uma vez
            if (NoMaisAlto < alturaArvore(saida, raiz)){
                NoMaisAlto = alturaArvore(saida, raiz);
            }
            free(dados.tecOrigem.nomeString); //  Libera as nomeStrings variaveis
            free(dados.tecDestino.nomeString);
        }
        else if (dados.removido == '1')
        {
            fseek(entrada, TAM_REGISTRO - 1, SEEK_CUR); // Pula o registro removido            }
        }
    }
    // atualizaHeader(saida, '1', -1, &RRNNoINdice);
    if (!encontrado)
    { // registro inexistente
        printf("Falha no processamento do arquivo.\n");
    }
    atualizaHeader(saida, '1', -1, &RRNNoINdice);
    No *raiz = criarNo();
    raiz = resgatarRaiz(saida, raiz);
    fclose(entrada);
    fclose(saida);
    binarioNaTela(saida);
}

int func6(FILE *arquivoDados, FILE *arquivoIndice, char *busca)
{

    cabecalhoArvore cabecalho;


    lerCabecalho(&cabecalho, arquivoIndice);
    //Verifica se algo foi lido no cabeçalho/se ele está consistente
    if (cabecalho.status != '1')
    {
        printf("Falha no processamento do arquivo.\n");
        return -1;
    }

    // variável que recebe o valor do rrn buscado de acordo com a chave (busca) passada
    int rrnBuscado = encontrarRRN(busca, cabecalho.noRaiz, arquivoIndice);
    if (rrnBuscado == -1)
    {   
        printf("Registro inexistente.\n");      //nao foi encontrado
    }
    else
    {
        func4(arquivoDados, rrnBuscado);
    }

    return 0;
}

int func7(FILE *arquivoDados, FILE *arquivoIndice, int n)
{
    Dados registroAux;
    cabecalhoArvore cabecalho;
    char nulo[4] = "NULO";

    lerCabecalho(&cabecalho, arquivoIndice);
    if (cabecalho.status != '1')
    {
        printf("Falha no processamento do arquivo.\n");
        return -1;
    }
    char grupo[4];
    char peso[4];
    char pop[4];

    for (int i = 0; i < n; i++)
    {

        scanf(" %[^,],%d,%d, %[^,],%d", registroAux.tecOrigem.nomeString, &registroAux.grupo, &registroAux.pop, registroAux.tecDestino.nomeString, &registroAux.peso);
        int comp1 = strcmp(nulo, registroAux.tecOrigem.nomeString);
        int comp2 = strcmp(nulo, registroAux.tecDestino.nomeString);
 

        if(comp1 == 0){
            registroAux.tecOrigem.tamString = 0;
            registroAux.tecOrigem.nomeString[0] = '\0';
        }
        if(comp2 == 0){
            registroAux.tecDestino.tamString = 0;
            registroAux.tecDestino.nomeString[0] = '\0';
        }

        atualizarIndices(&registroAux, arquivoDados, arquivoIndice);
    }
}