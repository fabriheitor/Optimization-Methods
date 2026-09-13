#include <stdio.h>
#include <stdlib.h>

#define MAX_PONTOS 1000
#define MAX_CONF 20
#define MAX_POS 4
#define MAX_POS_GLOBAL (MAX_PONTOS * MAX_POS)


typedef struct solPRCP{
    int vet_sol[MAX_PONTOS];
    int fo;
} Solucao;

int N;
int M;
int pos;
int mat_conf[MAX_POS_GLOBAL][MAX_CONF];
int vet_qtd_conf[MAX_POS_GLOBAL];
int num_pos;

void ler_dados(char* arq);
void testar_dados(char* arq);
void calcular_FO(Solucao& s);
void escrever_sol(Solucao& s, char* arq);