#include <stdio.h>
#include <stdlib.h>

#define MAX_VEICULOS 6
#define MAX_REQ 50
#define MAX_LOC 102
#define MAX_TEMPO 1440
#define MAX_CAP_VEI 3

typedef struct SolucaoDARP{
    int fo;
    int mat_sol[MAX_VEICULOS][MAX_LOC];
} Solucao;

int veiculos;
int req;
int locais;
int dur_max_rota;
int tempo_max_viagem;
int tempo_max_espera;
int vet_cap_vei[MAX_VEICULOS];
int vet_num_ass_loc[MAX_LOC];
int vet_tempo_servico[MAX_LOC];
int vet_ini_jan_tempo[MAX_TEMPO];
int vet_fin_jan_tempo[MAX_TEMPO];
int mat_tempo_desloc[MAX_LOC][MAX_LOC];


void testar_dados(char* arq);
void ler_dados(char* arq);
void calcular_FO(Solucao& s);
void escrever_sol(Solucao& s);