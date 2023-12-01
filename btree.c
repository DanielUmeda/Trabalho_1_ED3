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

//prototipo para inserção de um nó. --Não implementado--

int inserirNo(Dados *aInserir, int rrnAtual, FILE *arquivoIndice)
{
    fseek(arquivoIndice, (rrnAtual + 1) * 205, SEEK_SET);

    char busca[55];
    strcpy(busca, aInserir->tecOrigem.nomeString);
    strcat(busca, aInserir->tecDestino.nomeString);

    No no;
    lerNo(&no, arquivoIndice);

    for (int i = 0; i <= 3; i++)
    {
        int resultado = strcmp(busca, no.C[i]);

        if (i == 3)
        {
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
int inserirNoRec(Dados *aInserir, int rrnDaRaiz, FILE *arquivoIndice)
{
}

/*
void inserirNoNó(No no){
    /*short naoAchouAPosicao;
    int k;
    k = no.nroChavesNo;
    naoAchouAPosicao = k > 0;
    while(naoAchouAPosicao){
        if(no.C[k] >= no.P[k-1]){
            naoAchouAPosicao = FALSE;
            break;
        }
        no.P[k] = no.P[k-1];
        no.C[k+1] = no.C[k];
        k--;
        if(k < 1){
            naoAchouAPosicao = FALSE;
        }
    }
    no.P[k] = no;
    no.C[k+1] = apDir;
    no.nroChavesNo++;

}

// Função para inserir um registro na árvore-B
void inserirNaArvoreB(FILE *arquivoIndice, DadosArvoreB *registro, CabecalhoArvoreB *cabecalho) {
    // Implementação da lógica de inserção na árvore-B
    /*
    long i = 1;
    long j;
    Apontador apTemp;
    if(ap == NULL){
        *Cresceu = TRUE;
        *regRetorno = Reg;
        *apRetorno = NULL;
        return;
    }
    while(i < ap->n && Reg.Chave > ap->r[i-1].Chave){
        i++;
    }
    if(Reg.Chave < ap->r[i-1].Chave){
        //Erro: registro ja esta presente
    }
    if(Reg.Chave < ap->r[i-1].Chave){
        i--;
    }
    inserirNaArvoreB(Reg, ap->p[i], Cresceu, regRetorno, apRetorno);
    if(!*Cresceu){
        return;
    }
    if(ap->n < mm){
        InserirNoNó(ap, *regRetorno, *apRetorno);
        *Cresceu = FALSE;
        return;
    }
    //Overflow: no tem que ser dividido
    apTemp = (Apontador)malloc(sizeof(Nó));
    apTemp->n = 0;
    apTemp->p[0] = NULL;
    if(i < m+1){
        InserirNoNó(apTemp, ap->r[j-1],ap->p[j]);
        ap->n = m;
        apTemp->p[0] = ap->p[m+1];
        *regRetorno = ap->r[m];
        *apRetorno = apTemp;
    }

}

void insere(Registro Reg, Apontador ap){
    /*
    short Cresceu;
    Registro regRetorno;
    No *apRetorno, apTemp;
    InserirNaArvoreB(Reg, *ap, &Cresceu, &regRetorno, &apRetorno);
    if(Cresceu){
        apTemp = (No*)malloc(sizeof(No));
        apTemp->n = 1;
        apTemp->r[0] = regRetorno;
        apTemp->p[1] = apRetorno;
        apTemp->p[0] - *ap;
        *ap = apTemp;
    }

}
*/