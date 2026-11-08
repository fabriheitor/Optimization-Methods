#ifndef PROVA3_H
#define PROVA3_H

#define MAX_NOS 500

const double BETA = 1.0;
const double ALFA = 0.75;
const double LAMBDA = 1.0;


typedef struct tSolucao{
    int vet_sol[MAX_NOS];
    double fo;
}Solucao;

int num_nos;
int num_hubs;
double vet_X[MAX_NOS];
double vet_Y[MAX_NOS];
double mat_custo[MAX_NOS][MAX_NOS];
double vet_dist_total[MAX_NOS];
int vet_ind_nos_ord[MAX_NOS];


void heu_bl_rand(Solucao& s, const int iter);
void heu_bl_rand(Solucao& s);
void heu_bl_pm(Solucao& s);
void heu_bl_mm(Solucao& s);
void heu_bl_pm_nd(Solucao& s);


void gerar_vizinha(Solucao& s);
void heu_con_ale(Solucao& s);
void heu_con_gul(Solucao& s);
void heu_con_ale_gul(Solucao& s, const double per_ale);
void ordenar_objetos();


void calcular_FO(Solucao& s);
void escrever_sol(Solucao& s);
void testar_dados(char* arq);
void ler_dados(char* arq);

#endif

