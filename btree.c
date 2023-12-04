#include "btree.h"

No *criarNo()
{
    No *novoNo = (No *)malloc(sizeof(No));
    novoNo->nroChavesNo = 0;
    novoNo->alturaNo = 0;
    novoNo->RRNdoNo = -1;

    for (int i = 0; i < M; i++)
    {
        novoNo->P[i] = -1;  // Setando os ponteiros p/ nós filhos como nulos (ainda não há nenhum)
        novoNo->Pr[i] = -1; // Valor inicial da referência no arquivo de dados.
        for (int j = 0; j < M - 1; j++)
        {
            // inicialmente, o nó ainda não possui nenhuma chave associada
            novoNo->C[j][0] = '\0';
        }
    }
    return novoNo;
}

void lerCabecalho(cabecalhoArvore *cabecalho, FILE *arquivoIndice)
{

    fread(&cabecalho->status, sizeof(char), 1, arquivoIndice);
    fread(&cabecalho->noRaiz, sizeof(int), 1, arquivoIndice);
    fread(&cabecalho->RRNproxNo, sizeof(int), 1, arquivoIndice);
    fread(&cabecalho->lixo, sizeof(char), 196, arquivoIndice);
    // Chegou no fim do cabeçalho
}
void lerNo(No *no, FILE *arquivoIndice)
{

    fread(&no->nroChavesNo, sizeof(int), 1, arquivoIndice);
    fread(&no->alturaNo, sizeof(int), 1, arquivoIndice);
    fread(&no->RRNdoNo, sizeof(int), 1, arquivoIndice);

    for (int i = 0; i < M; i++)
    {
        fread(&(no->P[i]), sizeof(int), 1, arquivoIndice);

        if (i < M - 1)
        {
            fread(&(no->C[i]), sizeof(char), 55, arquivoIndice);
            fread(&(no->Pr[i]), sizeof(int), 1, arquivoIndice);
        }
    }
    for (int j = 0; j < 3; j++)
    {
        for (int i = 0; i < 55; i++)
        {
            if (no->C[j][i] == '$')
            {
                no->C[j][i] = '\0';
            }
        }
    }
}

// função recursiva para encontrar o RRN
int encontrarRRNRec(char *busca, int rrnAtual, FILE *arquivoIndice)
{
    fseek(arquivoIndice, (rrnAtual + 1) * 205, SEEK_SET);

    No no;
    lerNo(&no, arquivoIndice);

    /*
    Para ficar mais fácil a verificação, estamos sempre levando em conta o ponteiro à esquerda da chave. No caso C[3] não existe, pois são apenas 3 chaves por nó
    No entanto, ao utilizar i = 3, podemos verificar seu "filho" à esquerda, que seria o filho à direita de C[2].
     
    */
    for (int i = 0; i <= 3; i++)
    {
        int resultado = strcmp(busca, no.C[i]);

        if (i == 3)
        {   
            //No caso que i = 3, não há filhos à direita, então só podem estar à esquerda
            if (no.P[i] != -1)
            {
                return encontrarRRNRec(busca, no.P[i], arquivoIndice);
            }
            else
            {
                return -1;
            }
        }
        if (resultado < 0 || no.C[i][0] == '\0')
        {
            // busca é menor que c[i], logo deve estar à esquerda
            if (no.P[i] != -1)
            {
                return encontrarRRNRec(busca, no.P[i], arquivoIndice);
            }
            else
            {
                return -1;
            }
        }
        else if (resultado == 0)
        {
            // busca é igual à c[i], retornar a referência
            return no.Pr[i];
        }
    }
    if (no.P[no.nroChavesNo] != -1)
    {
        return encontrarRRNRec(busca, no.P[no.nroChavesNo], arquivoIndice);
    }
    else
    {
        // Se não houver último filho, a busca não foi encontrada
        return -1;
    }
}

// função auxiliar para chamar a função principal recursiva.
int encontrarRRN(char *busca, int rrnDaRaiz, FILE *arquivoIndice)
{
    return encontrarRRNRec(busca, rrnDaRaiz, arquivoIndice);
}

void escreverLixo(char vetor[])
{
    for (int i = 0; i < 55; i++)
    {
        vetor[i] = '$';
    }
}

// retorna caso esteja disponível à inserção comum dentro de um nó
static int estaDisponivel(No *no)
{
    if (no->C[2][0] != '$')
    {
        return 0; // Texto  existe, portanto NÃO está disponível
    }
    return 1; // Texto não existe, é vazio, portanto ESTÁ disponível
}



/* Insere o valor de fato no local pedido, só para o primeiro elemento da árvore */



//  função  de inserção comum seja no nó folha, intermediário, ou raíz no momento em que não há split e só inserção comum
static void insereNo(No *no, char *aux, int RRNReferencia, int lugar)
{
    switch (lugar)
    {
    case 1:
        // shiftar os aux
        strcpy(no->C[2], no->C[1]);
        strcpy(no->C[1], no->C[0]);
        strcpy(no->C[0], aux);

        // shift dos PRi
        no->Pr[2] = no->Pr[1];
        no->Pr[1] = no->Pr[0];
        no->Pr[0] = RRNReferencia;

        break;
    case 2:
        // shiftar os aux
        strcpy(no->C[2], no->C[1]);
        strcpy(no->C[1], aux);

        // shift dos PRi
        no->Pr[2] = no->Pr[1];
        no->Pr[1] = RRNReferencia;

        break;
    case 3:
        // shiftar os aux
        no->Pr[2] = RRNReferencia;

        // shift dos PRi
        strcpy(no->C[2], aux);
        break;
    case 4:
        printf("\n\n\ttestar se faz sentido ter estrado aqui");
        strcpy(no->C[0], aux);
        break;
    default:
        break;
    }
}

/* Insere o valor de fato no local pedido, só para o primeiro elemento da árvore */
static void inserirNoLugar(No *no, char *aux, int RRNReferencia, int lugar)
{
    switch (lugar)
    {
    case 1:
        strcpy(no->C[0], aux);
        no->Pr[0] = RRNReferencia;
        break;
    case 2:
        strcpy(no->C[1], aux);
        no->Pr[1] = RRNReferencia;
        break;
    case 3:
        strcpy(no->C[2], aux);
        no->Pr[2] = RRNReferencia;
        break;
    default:
        break;
    }
}

//  Muda os ponteiros em um split comum sem promoção dupla
static void mudaPonteiro(No *dadoNo, No *novoDireito, int lugar)
{
    switch (lugar)
    {
    case 1:

        dadoNo->P[3] = dadoNo->P[2];
        dadoNo->P[2] = dadoNo->P[1];
        dadoNo->P[1] = novoDireito->RRNdoNo;
        break;
    case 2:
        dadoNo->P[3] = dadoNo->P[2];
        dadoNo->P[2] = novoDireito->RRNdoNo;
        break;
    case 3:
        dadoNo->P[3] = novoDireito->RRNdoNo;
        break;
    default:
        break;
    }
}

//  Shift específico de ponteiros utilizado no momento de split intermediário como o da raíz ou nó não folha
static void mudaPonteiroSplit(No *noEsquerdo, No *inferiorDireito, No *superiorDireito, int *pointers, int lugar)
{

    switch (lugar)
    {
    case 1:

        superiorDireito->P[0] = pointers[2];
        superiorDireito->P[1] = pointers[3];

        noEsquerdo->P[1] = inferiorDireito->RRNdoNo;
        noEsquerdo->P[2] = pointers[1];

        noEsquerdo->P[3] = -1;
        break;
    case 2:
        superiorDireito->P[0] = pointers[2];
        superiorDireito->P[1] = pointers[3];

        noEsquerdo->P[2] = inferiorDireito->RRNdoNo;
        noEsquerdo->P[3] = -1;
        break;
    case 3:
        superiorDireito->P[0] = inferiorDireito->RRNdoNo;
        superiorDireito->P[1] = pointers[3];

        noEsquerdo->P[3] = -1;
        break;
    case 4:
        superiorDireito->P[0] = pointers[3];
        superiorDireito->P[1] = inferiorDireito->RRNdoNo;

        noEsquerdo->P[3] = -1;
        break;
    default:
        break;
    }
}


// Função acessora para pegar as chaves atuais do nó e repassar para o nó acima
char **promoverVetor(No *no, char **vetor, char *aux)
{
    int onde = ondeInserir(no, aux);
    switch (onde)
    {
    case 1:
        vetor[0] = strdup(aux);
        vetor[1] = strdup(no->C[0]);
        vetor[2] = strdup(no->C[1]);
        vetor[3] = strdup(no->C[2]);

        break;
    case 2:
        vetor[0] = strdup(no->C[0]);
        vetor[1] = strdup(aux);
        vetor[2] = strdup(no->C[1]);
        vetor[3] = strdup(no->C[2]);

        break;
    case 3:
        vetor[0] = strdup(no->C[0]);
        vetor[1] = strdup(no->C[1]);
        vetor[2] = strdup(aux);
        vetor[3] = strdup(no->C[2]);

        break;
    case 4:
        vetor[0] = strdup(no->C[0]);
        vetor[1] = strdup(no->C[1]);
        vetor[2] = strdup(no->C[2]);
        vetor[3] = strdup(aux);

        break;

    default:
        printf("erro 75\n");
        break;
    }
    return vetor;
}

// Função que compara as chaves atuais de um nó com uma string e fala onde ela devia ficar
int ondeInserir(No *no, char *aux)
{
    if (strcmp(aux, no->C[0]) < 0 || no->C[0][0] == '$')
        return 1;
    else if ((strcmp(aux, no->C[0]) > 0 && strcmp(aux, no->C[1]) < 0) || no->C[1][0] == '$')
        return 2;
    else if ((strcmp(aux, no->C[1]) > 0 && strcmp(aux, no->C[2]) < 0) || no->C[2][0] == '$')
        return 3;
    else if (strcmp(aux, no->C[2]) > 0)
        return 4;
    else
        return -1;
}


// Insere o nó no arquivo na posição em que estiver o arquivo de indice
// lugar == -1  para armazenar no final
void atualizarArquivo(FILE *arquivoIndice, No *no, int RRNLugar)
{
    no->alturaNo = alturaArvore(arquivoIndice, no);
    char aux = '$';
    if (RRNLugar != -1)
        fseek(arquivoIndice, (RRNLugar + 1) * TAMANHO_REGISTRO, SEEK_SET);
    else
        fseek(arquivoIndice, 0, SEEK_END);

    fwrite(&no->nroChavesNo, sizeof(int), 1, arquivoIndice);
    fwrite(&no->alturaNo, sizeof(int), 1, arquivoIndice);
    fwrite(&no->RRNdoNo, sizeof(int), 1, arquivoIndice);

    fwrite(&no->P[0], sizeof(int), 1, arquivoIndice);
    int teste = 1;
    for (int i = 0; i < 55; i++)
    {
        if (!teste)
        {
            teste = 0;
            fwrite(&aux, sizeof(char), 1, arquivoIndice);
        }
        if (teste)
        {
            if (no->C[0][i] == '\0')
            {
                teste = 0;
                fwrite(&aux, sizeof(char), 1, arquivoIndice);
            }
        }
        if (teste)
        {
            fwrite(&no->C[0][i], sizeof(char), 1, arquivoIndice);
        }
    }
    fwrite(&no->Pr[0], sizeof(int), 1, arquivoIndice);
    fwrite(&no->P[1], sizeof(int), 1, arquivoIndice);

    teste = 1;
    for (int i = 0; i < 55; i++)
    {
        if (!teste)
        {
            fwrite(&aux, sizeof(char), 1, arquivoIndice);
        }
        if (teste)
        {
            if (no->C[1][i] == '\0')
            {
                teste = 0;
                fwrite(&aux, sizeof(char), 1, arquivoIndice);
            }
        }
        if (teste)
        {
            fwrite(&no->C[1][i], sizeof(char), 1, arquivoIndice);
        }
    }
    fwrite(&no->Pr[1], sizeof(int), 1, arquivoIndice);
    fwrite(&no->P[2], sizeof(int), 1, arquivoIndice);

    teste = 1;
    for (int i = 0; i < 55; i++)
    {
        if (!teste)
        {
            teste = 0;
            fwrite(&aux, sizeof(char), 1, arquivoIndice);
        }
        if (teste)
        {
            if (no->C[2][i] == '\0')
            {
                teste = 0;
                fwrite(&aux, sizeof(char), 1, arquivoIndice);
            }
        }
        if (teste)
        {
            fwrite(&no->C[2][i], sizeof(char), 1, arquivoIndice);
        }
    }
    fwrite(&no->Pr[2], sizeof(int), 1, arquivoIndice);
    fwrite(&no->P[3], sizeof(int), 1, arquivoIndice);
    fflush(arquivoIndice);
}


//  Retorna o Pr de um nó específico
int *prVetor(No *noFilho, int *prPromovido, char *aux, int RRNReferencia)
{
    int onde = ondeInserir(noFilho, aux);
    switch (onde)
    {
    case 1:
        prPromovido[0] = RRNReferencia;
        prPromovido[1] = noFilho->Pr[0];
        prPromovido[2] = noFilho->Pr[1];
        prPromovido[3] = noFilho->Pr[2];

        break;
    case 2:

        prPromovido[0] = noFilho->Pr[0];
        prPromovido[1] = RRNReferencia;
        prPromovido[2] = noFilho->Pr[1];
        prPromovido[3] = noFilho->Pr[2];

        break;
    case 3:
        prPromovido[0] = noFilho->Pr[0];
        prPromovido[1] = noFilho->Pr[1];
        prPromovido[2] = RRNReferencia;
        prPromovido[3] = noFilho->Pr[2];

        break;
    case 4:
        prPromovido[0] = noFilho->Pr[0];
        prPromovido[1] = noFilho->Pr[1];
        prPromovido[2] = noFilho->Pr[2];
        prPromovido[3] = RRNReferencia;
        break;

    default:
        printf("erro 75\n");
        break;
    }
    return prPromovido;
}

// split comum no nó folha repassando o nó da direita e promovendo uma chave
static No *dividirNo(FILE *arquivoIndice, No *noFilho, No *novoDireito, char *aux, int RRNReferencia)
{
    char **vetor = (char **)malloc(4 * sizeof(char *));
    int *prPromovido = (int *)malloc(4 * sizeof(int));
    vetor = promoverVetor(noFilho, vetor, aux);
    prPromovido = prVetor(noFilho, prPromovido, aux, RRNReferencia);

    strcpy(noFilho->C[0], vetor[0]);
    strcpy(noFilho->C[1], vetor[1]);
    noFilho->Pr[0] = prPromovido[0];
    noFilho->Pr[1] = prPromovido[1];
 
    strcpy(novoDireito->C[0], vetor[3]);
    novoDireito->Pr[0] = prPromovido[3];

    escreverLixo(noFilho->C[2]);
    noFilho->Pr[2] = -1;
    atualizarArquivo(arquivoIndice, noFilho, noFilho->RRNdoNo);
    atualizarArquivo(arquivoIndice, novoDireito, novoDireito->RRNdoNo);

    novoDireito->auxPromovido = vetor;
    novoDireito->prPromovido = prPromovido;
    return novoDireito;
}


// Função acessora ao split carregando os ponteiros atuais
int *promoverPonteiros(No *no, int ponteiros[])
{
    ponteiros[0] = no->P[0];
    ponteiros[1] = no->P[1];
    ponteiros[2] = no->P[2];
    ponteiros[3] = no->P[3];
    return ponteiros;
}

// Testa se é raiz por meio da variavel local de "altura" e testa com a quantidade de filhos de um nó genérico
int eRaiz(FILE *arquivoIndice, No *raiz, int *NoMaisAlto)
{
    int alturaLocal = alturaArvore(arquivoIndice, raiz);
    if (alturaLocal == 1 && alturaLocal < *NoMaisAlto)
    {
        return 0; //  É nó folha
    }
    else if (*NoMaisAlto == alturaLocal)
    {
        return 1; //  É nó Raiz
    }

    else
    {
        return 0; //  É nó intermediario
    }
}


//  Lê um registro genérico em um RRN dentro do arquivo de índice
No *lerRegistroIndice(FILE *arquivoIndice, No *prNo, int RRN)
{

    fseek(arquivoIndice, (1 + RRN) * TAMANHO_REGISTRO, SEEK_SET);
    fread(&prNo->nroChavesNo, sizeof(int), 1, arquivoIndice);
    fread(&prNo->alturaNo, sizeof(int), 1, arquivoIndice);
    fread(&prNo->RRNdoNo, sizeof(int), 1, arquivoIndice);

    fread(&prNo->P[0], sizeof(int), 1, arquivoIndice);

    fread(&prNo->C[0], sizeof(char), 55, arquivoIndice);
    if (prNo->C[0][0] != '$')
    {
        for (int i = 0; i < 55; i++)
        {
            if (prNo->C[0][i] == '$')
            {
                prNo->C[0][i] = '\0';
                break;
            }
        }
    }
    else
        escreverLixo(prNo->C[0]);

    fread(&prNo->Pr[0], sizeof(int), 1, arquivoIndice);
    fread(&prNo->P[1], sizeof(int), 1, arquivoIndice);

    fread(&prNo->C[1], sizeof(char), 55, arquivoIndice);
    if (prNo->C[1][0] != '$')
    {
        for (int i = 0; i < 55; i++)
        {
            if (prNo->C[1][i] == '$')
            {
                prNo->C[1][i] = '\0';
                break;
            }
        }
    }
    else
        escreverLixo(prNo->C[1]);
    fread(&prNo->Pr[1], sizeof(int), 1, arquivoIndice);
    fread(&prNo->P[2], sizeof(int), 1, arquivoIndice);

    fread(&prNo->C[2], sizeof(char), 55, arquivoIndice);
    if (prNo->C[2][0] != '$')
    {
        for (int i = 0; i < 55; i++)
        {
            if (prNo->C[2][i] == '$')
            {
                prNo->C[2][i] = '\0';
                break;
            }
        }
    }
    else
        escreverLixo(prNo->C[2]);

    fread(&prNo->Pr[2], sizeof(int), 1, arquivoIndice);
    fread(&prNo->P[3], sizeof(int), 1, arquivoIndice);
    return prNo;
}


//  Retorna a altura do nó mais alto
int alturaArvore(FILE *arquivoIndice, No *no)
{
    int altura = 0;
    No *noFilho = criarNo();

    if (no->P[0] != -1)
    {
        noFilho = lerRegistroIndice(arquivoIndice, noFilho, no->P[0]);
        int alturaFilho = alturaArvore(arquivoIndice, noFilho);
        if (alturaFilho > altura)
        {
            altura = alturaFilho;
        }
    }
    if (no->P[1] != -1)
    {
        noFilho = lerRegistroIndice(arquivoIndice, noFilho, no->P[1]);

        int alturaFilho = alturaArvore(arquivoIndice, noFilho);
        if (alturaFilho > altura)
        {
            altura = alturaFilho;
        }
    }
    if (no->P[2] != -1)
    {
        noFilho = lerRegistroIndice(arquivoIndice, noFilho, no->P[1]);
        int alturaFilho = alturaArvore(arquivoIndice, noFilho);
        if (alturaFilho > altura)
        {
            altura = alturaFilho;
        }
    }
    if (no->P[3] != -1)
    {
        noFilho = lerRegistroIndice(arquivoIndice, noFilho, no->P[3]);
        int alturaFilho = alturaArvore(arquivoIndice, noFilho);
        if (alturaFilho > altura)
        {
            altura = alturaFilho;
        }
    }
    return altura + 1;
}

// Retorna o nó raiz proveniete do cabeçalho
No *resgatarRaiz(FILE *arquivoIndice, No *raiz)
{
    int valorNoRaiz;
    char status;
    fseek(arquivoIndice, 0, SEEK_SET); // Vai para o começo do arquivo
    fread(&status, sizeof(char), 1, arquivoIndice);
    fread(&valorNoRaiz, sizeof(int), 1, arquivoIndice); // Lê a raiz
    if (valorNoRaiz == -1)
        valorNoRaiz = 0;
    raiz = lerRegistroIndice(arquivoIndice, raiz, valorNoRaiz);
    return raiz;
}


// Função acessora ao split intermediário - quando recebe uma chave promovida proveniente de um split abaixo e retorna uma nova promoção acima
No *splitIntermediario(FILE *arquivoIndice, No *raiz, No *promovidoNoDireito, char **promovido, int *prPromovido, char *aux, int RRNReferencia, int *RRNNoINdice)
{
    No *novoDireitoInferior = promovidoNoDireito; //  Inicializa o novo nó direito inferior
    No *novoDireitoSuperior = criarNo();       //  Inicializa o novo nó direito superior

    novoDireitoSuperior->RRNdoNo = *RRNNoINdice;
    (*RRNNoINdice)++;

    int pointers[4];
    promoverPonteiros(raiz, pointers); 

    promovido = novoDireitoInferior->auxPromovido;   //  Vetor auxiliar para promoção
    prPromovido = novoDireitoInferior->prPromovido; //  Vetor auxiliar para promoção PRI
    char *promovidoInferior = promovido[2];
    int prPromovidoInferior = prPromovido[2];
    promovido = promoverVetor(raiz, promovido, promovidoInferior);
    prPromovido = prVetor(raiz, prPromovido, promovidoInferior, prPromovidoInferior);
    // Auxilia na promoção, ponteiros completos
    int onde = ondeInserir(raiz, promovidoInferior);
    novoDireitoSuperior = dividirNo(arquivoIndice, raiz, novoDireitoSuperior, promovidoInferior, prPromovidoInferior); //  Splita o nó da direita

    mudaPonteiroSplit(raiz, novoDireitoInferior, novoDireitoSuperior, pointers, onde); // splita os ponteiros no formato de promoção
    atualizarArquivo(arquivoIndice, raiz, raiz->RRNdoNo);
    atualizarArquivo(arquivoIndice, novoDireitoSuperior, novoDireitoSuperior->RRNdoNo);

    novoDireitoSuperior->auxPromovido = promovido;
    novoDireitoSuperior->prPromovido = prPromovido;
    return novoDireitoSuperior;
}

// Atualiza o header do arquivo  indice com as informações abaixo
void atualizaHeader(FILE *arquivoIndice, char status, int RRNNoRaiz, int *RRNNoINdice)
{
    fseek(arquivoIndice, 0, SEEK_SET);

    fwrite(&status, sizeof(char), 1, arquivoIndice);
    if (RRNNoRaiz != -1)
    {
        fwrite(&RRNNoRaiz, sizeof(int), 1, arquivoIndice);
    }
    else
    {
        fseek(arquivoIndice, 4, SEEK_CUR);
    }
    fwrite(RRNNoINdice, sizeof(int), 1, arquivoIndice);
    fflush(arquivoIndice);
}


// Caso o nó não tenha mais filhos na direção desejada, insere ali mesmo ou promove através do split
No *insereLocal(FILE *arquivoIndice, No *raiz, char **promovido, char *aux, int RRNReferencia, int *RRNNoINdice, int *NoMaisAlto)
{
    if (estaDisponivel(raiz))
    {
        insereNo(raiz, aux, RRNReferencia, ondeInserir(raiz, aux));
        atualizarArquivo(arquivoIndice, raiz, raiz->RRNdoNo);
        return NULL;
    }
    else // split case
    {
        if (eRaiz(arquivoIndice, raiz, NoMaisAlto)) // split na raiz
        {
            int *prPromovido = (int *)malloc(sizeof(int));
            No *novaRaiz = criarNo();               //  Inicializa um novo nó raiz
            No *novoDireito = criarNo();              //  Inicializa um novo nó direito
            promovido = promoverVetor(raiz, promovido, aux); //  Vetor auxiliar para promoção
            prPromovido = prVetor(raiz, prPromovido, aux, RRNReferencia);

            novoDireito->RRNdoNo = *RRNNoINdice;
            (*RRNNoINdice)++;
            novaRaiz->RRNdoNo = *RRNNoINdice;
            (*RRNNoINdice)++;

            novoDireito = dividirNo(arquivoIndice, raiz, novoDireito, aux, RRNReferencia); //  Splita o nó da direita
            // mudaPonteiro(novaRaiz, novoDireito, ondeInserir(novaRaiz, promovido[2])); //  Shifta, dessa vez, os ponteiros no local correto

            // Nomeia os filhos e atribui o valor da raíz
            novaRaiz->P[0] = raiz->RRNdoNo;
            novaRaiz->P[1] = novoDireito->RRNdoNo;
            strcpy(novaRaiz->C[0], promovido[2]);
            novaRaiz->Pr[0] = prPromovido[2];

            atualizarArquivo(arquivoIndice, novaRaiz, novaRaiz->RRNdoNo);
            atualizaHeader(arquivoIndice, '0', novaRaiz->RRNdoNo, RRNNoINdice);
            // novaRaizSplit(arquivoIndice, raiz, novoDireito, promovido, prPromovido, aux, RRNReferencia, RRNNoINdice);
            return NULL;
        }
        else
        {
            // Caso não seja nem raíz nem um nó folha disponível, promove um novo nó direito com o dado promovido
            No *novoDireito = criarNo();
            novoDireito->RRNdoNo = *RRNNoINdice;
            (*RRNNoINdice)++;
            // novoDireito->auxPromovido = promoverVetor(raiz, promovido, aux);
            // novoDireito->prPromovido = prVetor(raiz, prPromovido, aux, RRNReferencia);
            novoDireito = dividirNo(arquivoIndice, raiz, novoDireito, aux, RRNReferencia); //  Splita o o nó direito inferior
            atualizarArquivo(arquivoIndice, novoDireito, novoDireito->RRNdoNo);
            return novoDireito; // para caso tenha que dar split
        }
    }
}

// Split especial usado para dar split no nó quando este for raíz, criando um nó direito e um novo nó raiz
void novaRaizSplit(FILE *arquivoIndice, No *raiz, No *promovidoNoDireito, char **promovido, int *prPromovido, char *aux, int RRNReferencia, int *RRNNoINdice)
{
    No *novaRaiz = criarNo();              //  Inicializa a nova raiz
    No *novoDireitoInferior = promovidoNoDireito; //  Inicializa o novo nó direito superior
    No *novoDireitoSuperior = criarNo();       //  Inicializa o novo nó direito inferior

    // char *promovidoInferior = promovido[2]; //  Auxiliar para pegar o nó promovido do segundo abaixo

    novoDireitoSuperior->RRNdoNo = *RRNNoINdice;
    (*RRNNoINdice)++;
    novaRaiz->RRNdoNo = *RRNNoINdice;
    (*RRNNoINdice)++;

    int pointers[4];
    promoverPonteiros(raiz, pointers);

    char *promovidoInferior = novoDireitoInferior->auxPromovido[2];
    int pripromovidoInferior = novoDireitoInferior->prPromovido[2];
    promovido = promoverVetor(raiz, promovido, aux);                                 //  Vetor auxiliar para promoção
    prPromovido = prVetor(raiz, prPromovido, promovidoInferior, pripromovidoInferior); //  Vetor auxiliar para promoção PRI

    // Auxilia na promoção, ponteiros completos
    int onde = ondeInserir(raiz, promovidoInferior);
    novoDireitoSuperior = dividirNo(arquivoIndice, raiz, novoDireitoSuperior, promovidoInferior, pripromovidoInferior); //  Splita o nó da direita

    // mudaPonteiro(novaRaiz, novoDireito, ondeInserir(novaRaiz, promovido[2])); //  Shifta, dessa vez, os ponteiros no local correto
    mudaPonteiroSplit(raiz, novoDireitoInferior, novoDireitoSuperior, pointers, onde); // splita os ponteiros no formato de promoção
    // Nomeia os filhos e atribui o valor da raíz
    novaRaiz->P[0] = raiz->RRNdoNo;
    novaRaiz->P[1] = novoDireitoSuperior->RRNdoNo;
    strcpy(novaRaiz->C[0], promovido[2]);
    novaRaiz->Pr[0] = prPromovido[2];

    atualizarArquivo(arquivoIndice, raiz, raiz->RRNdoNo);
    atualizarArquivo(arquivoIndice, novoDireitoSuperior, novoDireitoSuperior->RRNdoNo);
    atualizarArquivo(arquivoIndice, novaRaiz, novaRaiz->RRNdoNo);
    atualizaHeader(arquivoIndice, '0', novaRaiz->RRNdoNo, RRNNoINdice);
}

// Função principal de inserção da string e do RRN na no arquivo Índice de árvore B
No *inserirStringIndice(FILE *arquivoIndice, int no_inIndex, char *aux, int *NoMaisAlto, int *RRNNoINdice, int RRNReferencia)
{
    char **promovido = (char **)malloc(4 * sizeof(char *)); // 4 para promoção das strings em si e 4 para promover as PRis
    int *prPromovido;
    No *novoDireito;
    No *raiz = criarNo();
    raiz = lerRegistroIndice(arquivoIndice, raiz, no_inIndex);

    if (raiz->C[0][0] == '$') // primeira inserção
    {
        inserirNoLugar(raiz, aux, RRNReferencia, 1);
        raiz->RRNdoNo = *RRNNoINdice;
        atualizarArquivo(arquivoIndice, raiz, 0);
        atualizaHeader(arquivoIndice, '0', raiz->RRNdoNo, RRNNoINdice);
        (*RRNNoINdice)++;
        return NULL;
    }

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

    // Inserir na 1ª posição
    else if (strcmp(aux, raiz->C[0]) < 0 || raiz->C[0][0] == '$')
    {
        if (raiz->P[0] != -1)
        {
            if ((novoDireito = inserirStringIndice(arquivoIndice, raiz->P[0], aux, NoMaisAlto, RRNNoINdice, RRNReferencia)) != NULL)
            {
                // se subiu, promoveu o filho
                // acessorSun(); // caso tenha um nó abaixo, repassa o código recursivo pra lá
                promovido = novoDireito->auxPromovido;
                prPromovido = novoDireito->prPromovido;
                char *promovidoInferior = promovido[2];
                if (estaDisponivel(raiz))
                {
                    No *noFilho = criarNo();
                    noFilho = lerRegistroIndice(arquivoIndice, noFilho, raiz->P[0]);
                    mudaPonteiro(raiz, novoDireito, ondeInserir(raiz, promovido[2]));
                    insereNo(raiz, promovidoInferior, prPromovido[2], ondeInserir(raiz, promovidoInferior));

                    atualizarArquivo(arquivoIndice, raiz, raiz->RRNdoNo);
                    // atualizarArquivo(arquivoIndice, noFilho, noFilho->RRNdoNo);
                    // atualizarArquivo(arquivoIndice, novoDireito, novoDireito->RRNdoNo);
                    // atualizaHeader(arquivoIndice, '1', raiz->RRNdoNo, RRNNoINdice);
                    return NULL;
                }
                else // split de novo
                {
                    if (eRaiz(arquivoIndice, raiz, NoMaisAlto)) // split na raiz
                    {
                        novaRaizSplit(arquivoIndice, raiz, novoDireito, promovido, prPromovido, promovidoInferior, RRNReferencia, RRNNoINdice);
                        return NULL;
                    }
                    else // split intermediário
                    {
                        return splitIntermediario(arquivoIndice, raiz, novoDireito, promovido, prPromovido, aux, RRNReferencia, RRNNoINdice);
                    }
                }
            }
        }
        else
        {
            // insere localmente no nó, caso esteja cheio faz o split e promove pelo return um no nó direito
            return insereLocal(arquivoIndice, raiz, promovido, aux, RRNReferencia, RRNNoINdice, NoMaisAlto);
        }
    }

    // Inserir na 2ª posição
    else if ((strcmp(aux, raiz->C[0]) > 0 && strcmp(aux, raiz->C[1]) < 0) || raiz->C[1][0] == '$')
    {
        if (raiz->P[1] != -1)
        {
            if ((novoDireito = inserirStringIndice(arquivoIndice, raiz->P[1], aux, NoMaisAlto, RRNNoINdice, RRNReferencia)) != NULL)
            {
                // se subiu, promoveu o filho
                // acessorSun(); // caso tenha um nó abaixo, repassa o código recursivo pra lá
                promovido = novoDireito->auxPromovido;
                prPromovido = novoDireito->prPromovido;
                char *promovidoInferior = promovido[2];
                if (estaDisponivel(raiz))
                {
                    No *noFilho = criarNo();
                    noFilho = lerRegistroIndice(arquivoIndice, noFilho, raiz->P[1]);
                    mudaPonteiro(raiz, novoDireito, ondeInserir(raiz, promovido[2]));
                    insereNo(raiz, promovidoInferior, prPromovido[2], ondeInserir(raiz, promovidoInferior));

                    atualizarArquivo(arquivoIndice, raiz, raiz->RRNdoNo);
                    // atualizarArquivo(arquivoIndice, noFilho, noFilho->RRNdoNo);
                    // atualizarArquivo(arquivoIndice, novoDireito, novoDireito->RRNdoNo);
                    // atualizaHeader(arquivoIndice, '1', raiz->RRNdoNo, RRNNoINdice);
                    return NULL;
                }
                else // split de novo
                {
                    if (eRaiz(arquivoIndice, raiz, NoMaisAlto)) // split na raiz
                    {
                        novaRaizSplit(arquivoIndice, raiz, novoDireito, promovido, prPromovido, promovidoInferior, RRNReferencia, RRNNoINdice);
                        return NULL;
                    }
                    else // split intermediário
                    {
                        return splitIntermediario(arquivoIndice, raiz, novoDireito, promovido, prPromovido, aux, RRNReferencia, RRNNoINdice);
                    }
                }
            }
        }
        else
        {
            // insere localmente no nó, caso esteja cheio faz o split e promove pelo return um no nó direito
            return insereLocal(arquivoIndice, raiz, promovido, aux, RRNReferencia, RRNNoINdice, NoMaisAlto);
        }
    }

    // Inserir na 3ª posição
    else if ((strcmp(aux, raiz->C[1]) > 0 && strcmp(aux, raiz->C[2]) < 0) || raiz->C[2][0] == '$')
    {
        if (raiz->P[2] != -1)
        {
            if ((novoDireito = inserirStringIndice(arquivoIndice, raiz->P[2], aux, NoMaisAlto, RRNNoINdice, RRNReferencia)) != NULL)
            {
                // se subiu, promoveu o filho
                promovido = novoDireito->auxPromovido;
                prPromovido = novoDireito->prPromovido;
                char *promovidoInferior = promovido[2];
                if (estaDisponivel(raiz))
                {
                    No *noFilho = criarNo();
                    noFilho = lerRegistroIndice(arquivoIndice, noFilho, raiz->P[2]);
                    mudaPonteiro(raiz, novoDireito, ondeInserir(raiz, promovido[2]));
                    insereNo(raiz, promovidoInferior, prPromovido[2], ondeInserir(raiz, promovidoInferior));

                    atualizarArquivo(arquivoIndice, raiz, raiz->RRNdoNo);
                    // atualizarArquivo(arquivoIndice, noFilho, noFilho->RRNdoNo);
                    // atualizarArquivo(arquivoIndice, novoDireito, novoDireito->RRNdoNo);
                    // atualizaHeader(arquivoIndice, '1', raiz->RRNdoNo, RRNNoINdice);
                    return NULL;
                }
                else // split de novo
                {
                    if (eRaiz(arquivoIndice, raiz, NoMaisAlto)) // split na raiz
                    {
                        novaRaizSplit(arquivoIndice, raiz, novoDireito, promovido, prPromovido, promovidoInferior, RRNReferencia, RRNNoINdice);
                        return NULL;
                    }
                    else // split intermediário
                    {
                        return splitIntermediario(arquivoIndice, raiz, novoDireito, promovido, prPromovido, aux, RRNReferencia, RRNNoINdice);
                    }
                }
            }
        }
        else
        {
            // insere localmente no nó, caso esteja cheio faz o split e promove pelo return um no nó direito
            return insereLocal(arquivoIndice, raiz, promovido, aux, RRNReferencia, RRNNoINdice, NoMaisAlto);
        }
    }

    // Inserir na ultima posição
    else if (strcmp(aux, raiz->C[2]) > 0)
    {

        if (raiz->P[3] != -1)
        {
            if ((novoDireito = inserirStringIndice(arquivoIndice, raiz->P[3], aux, NoMaisAlto, RRNNoINdice, RRNReferencia)) != NULL)
            {
                // se subiu, promoveu o filho
                // acessorSun(); // caso tenha um nó abaixo, repassa o código recursivo pra lá
                promovido = novoDireito->auxPromovido;
                prPromovido = novoDireito->prPromovido;
                char *promovidoInferior = promovido[2];
                if (estaDisponivel(raiz))
                {
                    No *noFilho = criarNo();
                    noFilho = lerRegistroIndice(arquivoIndice, noFilho, raiz->P[3]);
                    mudaPonteiro(raiz, novoDireito, ondeInserir(raiz, promovido[2]));
                    insereNo(raiz, promovidoInferior, prPromovido[2], ondeInserir(raiz, promovidoInferior));

                    atualizarArquivo(arquivoIndice, raiz, raiz->RRNdoNo);
                    // atualizarArquivo(arquivoIndice, noFilho, noFilho->RRNdoNo);
                    // atualizarArquivo(arquivoIndice, novoDireito, novoDireito->RRNdoNo);
                    // atualizaHeader(arquivoIndice, '1', raiz->RRNdoNo, RRNNoINdice);
                    return NULL;
                }
                else // split de novo
                {
                    if (eRaiz(arquivoIndice, raiz, NoMaisAlto)) // split na raiz
                    {
                        novaRaizSplit(arquivoIndice, raiz, novoDireito, promovido, prPromovido, promovidoInferior, RRNReferencia, RRNNoINdice);
                        return NULL;
                    }
                    else // split intermediário
                    {
                        return splitIntermediario(arquivoIndice, raiz, novoDireito, promovido, prPromovido, aux, RRNReferencia, RRNNoINdice);
                    }
                }
            }
        }
        else
        {
            // insere localmente no nó, caso esteja cheio faz o split e promove pelo return um no nó direito
            return insereLocal(arquivoIndice, raiz, promovido, aux, RRNReferencia, RRNNoINdice, NoMaisAlto);
        }
    }
    return NULL;
}


// Insere a primeira raiz
void inserePrimeiraRaiz(FILE *arquivoIndice)
{
    char aux = '$';
    No *novaRaiz = criarNo();
    novaRaiz->alturaNo = -1;
    novaRaiz->RRNdoNo = -1;
    novaRaiz->nroChavesNo = -1;
    fwrite(&novaRaiz->nroChavesNo, sizeof(int), 1, arquivoIndice);
    fwrite(&novaRaiz->alturaNo, sizeof(int), 1, arquivoIndice);
    fwrite(&novaRaiz->RRNdoNo, sizeof(int), 1, arquivoIndice);
    fwrite(&novaRaiz->P[0], sizeof(int), 1, arquivoIndice);
    for (int i = 0; i < 55; i++)
    {
        fwrite(&aux, sizeof(char), 1, arquivoIndice);
    }
    fwrite(&novaRaiz->Pr[0], sizeof(int), 1, arquivoIndice);
    fwrite(&novaRaiz->P[1], sizeof(int), 1, arquivoIndice);

    for (int i = 0; i < 55; i++)
    {
        fwrite(&aux, sizeof(char), 1, arquivoIndice);
    }
    fwrite(&novaRaiz->Pr[1], sizeof(int), 1, arquivoIndice);
    fwrite(&novaRaiz->P[2], sizeof(int), 1, arquivoIndice);

    for (int i = 0; i < 55; i++)
    {
        fwrite(&aux, sizeof(char), 1, arquivoIndice);
    }
    fwrite(&novaRaiz->Pr[2], sizeof(int), 1, arquivoIndice);
    fwrite(&novaRaiz->P[3], sizeof(int), 1, arquivoIndice);
}

// Testa, obtem aux e insere o dado dentro do arquivo de index com o RRNReferencia
void insereNoIndice(FILE *arquivoIndice, Dados *dados, int *NoMaisAlto, int *RRNNoINdice, int RRNReferencia)
{
    int tamanhoMaxString = strlen(dados->tecDestino.nomeString) + strlen(dados->tecOrigem.nomeString) + 1; //  Para se concatenar, achas-se o tamanho total da string concatenada
    char aux[tamanhoMaxString];                                                                                          //  Cria um auxiliar para guardar tal string concatenada
    strcpy(aux, dados->tecOrigem.nomeString);                                                                        // Copia origem na aux
    strcat(aux, dados->tecDestino.nomeString);
    // printf(" -> %s, %d\n", aux, RRNReferencia); // Concatena com destino
    fflush(stdout);

    No *raizReal = criarNo();
    raizReal = resgatarRaiz(arquivoIndice, raizReal);
    if (raizReal->RRNdoNo == -1)
        raizReal->RRNdoNo = 0;
    raizReal = inserirStringIndice(arquivoIndice, raizReal->RRNdoNo, aux, NoMaisAlto, RRNNoINdice, RRNReferencia);
}


