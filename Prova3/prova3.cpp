#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <math.h>
#include <float.h>
#include <time.h>

#include "prova3.h"

#define MAX(X,Y) ((X > Y) ? X : Y)
#define MIN(X,Y) ((X < Y) ? X : Y)

int main()
{
    srand(time(NULL));

    char arq[50];
    const char* caminhos[] = {"instancia.txt", "../instancia.txt", "Prova3/instancia.txt", "../Prova3/instancia.txt"};
    strcpy(arq, "instancia.txt");
    for (int c = 0; c < 4; c++)
    {
        FILE* teste = fopen(caminhos[c], "r");
        if (teste != NULL)
        {
            fclose(teste);
            strcpy(arq, caminhos[c]);
            break;
        }
    }

    ler_dados(arq);
    //strcpy(arq, "");
    //testar_dados(arq);

    ordenar_objetos();

    Solucao sra, smm, spm, spmnd;
    heu_con_ale(sra);
    calcular_FO(sra);
    memcpy(&smm, &sra, sizeof(Solucao));
    memcpy(&spm, &sra, sizeof(Solucao));
    memcpy(&spmnd, &sra, sizeof(Solucao));

    printf("\nOR: %.2f\n", sra.fo);
    heu_bl_rand(sra, 3 * num_hubs * (num_nos - num_hubs));
    heu_bl_mm(smm);
    heu_bl_pm(spm);
    heu_bl_pm_nd(spmnd);

    printf("RA: %.2f\tMM: %.2f\tPM: %.2f\tND: %.2f\n", sra.fo, smm.fo,
           spm.fo, spmnd.fo);

    printf("\nSolucao refinada (Melhor Melhora):");
    escrever_sol(smm);

    return 0;
}

void heu_bl_rand(Solucao& s, const int iter){
    Solucao aux;
    INICIO : ;
    for(int i = 0; i < iter; i++){
        memcpy(&aux, &s, sizeof(s));
        gerar_vizinha(aux);
        calcular_FO(aux);
        if(aux.fo < s.fo){
            memcpy(&s, &aux, sizeof(aux));
            goto INICIO;
        }
    }
}

void heu_bl_pm(Solucao& s){
    double mel_fo = s.fo;
    INICIO : ;
    for(int i = 0; i < num_hubs; i++){
        double hub_ori = s.vet_sol[i];
        double fo_ori = s.fo;
        for(int j = num_hubs; j < num_nos; j++){
            s.vet_sol[i] = s.vet_sol[j];
            s.vet_sol[j] = hub_ori;

            calcular_FO(s);
            if(s.fo < mel_fo){
                goto INICIO;
            }

            s.vet_sol[j] = s.vet_sol[i];
            s.vet_sol[i] = hub_ori;
            s.fo = fo_ori;
        }
    }
}

void heu_bl_mm(Solucao& s)
{
    int mel_fo, mel_pos_hub, mel_pos_nao_hub;
    mel_fo = s.fo;
    while(1)
    {
        int flag = 0;
        for(int i = 0; i < num_hubs; i++)
        {
            int hub_ori = s.vet_sol[i];
            int fo_ori = s.fo;
            for(int j = num_hubs; j < num_nos; j++)
            {
                s.vet_sol[i] = s.vet_sol[j];
                s.vet_sol[j] = hub_ori;
                calcular_FO(s);
                    if(s.fo < mel_fo)
                    {
                        mel_pos_hub = i;
                        mel_pos_nao_hub = j;
                        mel_fo = s.fo;
                        flag = 1;
                    }
                s.vet_sol[j] = s.vet_sol[i];
                s.vet_sol[i] = hub_ori;
                s.fo = fo_ori;
            }
        }
        if(flag)
        {
            int aux = s.vet_sol[mel_pos_hub];
            s.vet_sol[mel_pos_hub] = s.vet_sol[mel_pos_nao_hub];
            s.vet_sol[mel_pos_nao_hub] = aux;
            s.fo = mel_fo;
        }
        else
        {
            break;
        }
    }
}

// Nao-Deterministica 
void heu_bl_pm_nd(Solucao& s)
{
    int vet_aux[MAX_NOS];
    for (int i = 0; i < num_hubs; i++)
        vet_aux[i] = i;

    INICIO : ;
    double mel_fo = s.fo;
    for (int i = 0; i < num_hubs; i++)
    {
        int pos = i + rand() % (num_hubs - i);
        int ind_hub = vet_aux[pos];

        int hub_ori = s.vet_sol[ind_hub];
        double fo_ori = s.fo;
        for (int j = num_hubs; j < num_nos; j++)
        {
            s.vet_sol[ind_hub] = s.vet_sol[j];
            s.vet_sol[j] = hub_ori;

            calcular_FO(s);
            if (s.fo < mel_fo)
                goto INICIO;

            s.vet_sol[j] = s.vet_sol[ind_hub];
            s.vet_sol[ind_hub] = hub_ori;
            s.fo = fo_ori;
        }

        int aux = vet_aux[i];
        vet_aux[i] = vet_aux[pos];
        vet_aux[pos] = aux;
    }
}

// ============================================================================
// Geracao de Vizinha e Heuristicas Construtivas (adaptadas do PMM)
// ============================================================================
void gerar_vizinha(Solucao& s)
{
    // Sorteia um hub (0 .. p-1) e um nao-hub (p .. n-1) e faz a troca
    int pos_hub = rand() % num_hubs;
    int pos_nao_hub = num_hubs + rand() % (num_nos - num_hubs);

    int aux = s.vet_sol[pos_hub];
    s.vet_sol[pos_hub] = s.vet_sol[pos_nao_hub];
    s.vet_sol[pos_nao_hub] = aux;
}

void heu_con_ale(Solucao& s)
{
    for (int i = 0; i < num_nos; i++)
        s.vet_sol[i] = i;

    for (int i = 0; i < num_nos; i++)
    {
        int pos = i + rand() % (num_nos - i);
        int aux = s.vet_sol[i];
        s.vet_sol[i] = s.vet_sol[pos];
        s.vet_sol[pos] = aux;
    }
}

void heu_con_gul(Solucao& s)
{
    ordenar_objetos();
    for (int i = 0; i < num_nos; i++)
        s.vet_sol[i] = vet_ind_nos_ord[i];
}

void heu_con_ale_gul(Solucao& s, const double per_ale)
{
    ordenar_objetos();
    int vet_aux[MAX_NOS];
    memcpy(&vet_aux, &vet_ind_nos_ord, sizeof(vet_ind_nos_ord));
    int qtde = MAX(1, (per_ale / 100.0) * num_nos);
    for (int i = 0; i < qtde; i++)
    {
        int pos = i + rand() % (num_nos - i);
        int aux = vet_aux[i];
        vet_aux[i] = vet_aux[pos];
        vet_aux[pos] = aux;
    }
    for (int i = 0; i < num_nos; i++)
        s.vet_sol[i] = vet_aux[i];
}

void ordenar_objetos()
{
    // Ordena os nos em ordem crescente da distancia total para todos os demais
    // (nos mais centrais ficam nas primeiras posicoes para serem escolhidos como hubs)
    for (int i = 0; i < num_nos; i++)
        vet_ind_nos_ord[i] = i;

    int flag = 1;
    while (flag)
    {
        flag = 0;
        for (int j = 0; j < num_nos - 1; j++)
        {
            if (vet_dist_total[vet_ind_nos_ord[j]] > vet_dist_total[vet_ind_nos_ord[j + 1]])
            {
                int aux = vet_ind_nos_ord[j];
                vet_ind_nos_ord[j] = vet_ind_nos_ord[j + 1];
                vet_ind_nos_ord[j + 1] = aux;
                flag = 1;
            }
        }
    }
}

// ============================================================================
// Calculo de FO e Manipulacao de Dados
// ============================================================================
void calcular_FO(Solucao& s)
{
    s.fo = 0.0;

    // Para cada par de nos (i, j) em N x N:
    // O custo de transporte entre i e j e o MENOR custo passando pelos hubs (k, l) instalados.
    // A FO e o MAIOR custo t_ij dentre todos os pares (i, j).
    for (int i = 0; i < num_nos; i++)
    {
        for (int j = i; j < num_nos; j++)
        {
            double menor_t_ij = DBL_MAX;
            for (int h1 = 0; h1 < num_hubs; h1++)
            {
                int k = s.vet_sol[h1];
                double coleta = BETA * mat_custo[i][k];
                if (coleta >= menor_t_ij)
                    continue;

                for (int h2 = 0; h2 < num_hubs; h2++)
                {
                    int l = s.vet_sol[h2];
                    double custo = coleta + ALFA * mat_custo[k][l] + LAMBDA * mat_custo[l][j];
                    if (custo < menor_t_ij)
                        menor_t_ij = custo;
                }
            }
            if (menor_t_ij > s.fo)
                s.fo = menor_t_ij;
        }
    }
}

void escrever_sol(Solucao& s)
{
    printf("\nFO: %.2f\n", s.fo);
    printf("HUBS: [");
    for (int i = 0; i < num_hubs; i++)
        printf("%d%s", s.vet_sol[i], (i < num_hubs - 1) ? ", " : "");
    printf("]\n");
    printf("Vetor solucao (p hubs | n-p nao-hubs): ");
    for (int i = 0; i < num_nos; i++)
        printf("%d ", s.vet_sol[i]);
    printf("\n");
}

void testar_dados(char* arq)
{
    FILE* f;
    if (strcmp(arq, "") == 0)
        f = stdout;
    else
        f = fopen(arq, "w");

    fprintf(f, "%d %d\n", num_nos, num_hubs);
    for (int i = 0; i < num_nos; i++)
        fprintf(f, "%.2f %.2f\n", vet_X[i], vet_Y[i]);

    if (strcmp(arq, "") != 0)
        fclose(f);
}

void ler_dados(char* arq)
{
    FILE* f = fopen(arq, "r");
    if (f == NULL)
    {
        printf("Erro ao abrir o arquivo: %s\n", arq);
        exit(1);
    }
    fscanf(f, "%d %d", &num_nos, &num_hubs);
    for (int i = 0; i < num_nos; i++)
        fscanf(f, "%lf %lf", &vet_X[i], &vet_Y[i]);
    fclose(f);

    for (int i = 0; i < num_nos; i++)
    {
        vet_dist_total[i] = 0.0;
        for (int j = 0; j < num_nos; j++)
        {
            mat_custo[i][j] = sqrt(
                (vet_X[i] - vet_X[j]) * (vet_X[i] - vet_X[j]) +
                (vet_Y[i] - vet_Y[j]) * (vet_Y[i] - vet_Y[j])
            );
            vet_dist_total[i] += mat_custo[i][j];
        }
    }
}
