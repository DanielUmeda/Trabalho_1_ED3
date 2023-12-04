#ifndef INDICES_H
#define INDICES_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "btree.h"
#include "dados.h"

int atualizarIndices(Dados *registroASerEscrito, FILE *arquivoDados, FILE *arquivoIndices);
#endif
