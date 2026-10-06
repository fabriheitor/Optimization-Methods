#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include "prcp.h"


void ler_dados(char* arq){
    FILE* f = fopen(arq, "r");
    if(strcmp(arq,"") == 0){
        f = stdout;
    }

    fscanf(f, "%d", &N);
    fscanf(f, "%d", &M);
    num_pos = N * M;

    for(int i = 0; i < num_pos; i++){
        fscanf(f, "%d", &vet_qtd_conf[i]);
            for(int k = 0; k < vet_qtd_conf[i]; k++){
                fscanf(f, "%d", &mat_conf[i][k]);
                mat_conf[i][k]--;
            }
    }
}

void testar_dados(char* arq){
    FILE* f = fopen(arq, "w");
    if(strcmp(arq,"") == 0){
        f = stdout;
    }

    fprintf(f, "%d\n", N);
    fprintf(f, "%d\n", M);

    for(int i = 0; i < N; i++){
        fprintf(f, "%d\n", vet_qtd_conf[i]);
            for(int k = 0; k < vet_qtd_conf[i]; k++){
                fprintf(f, "%d ", mat_conf[i][k]);
            }
        fprintf(f,"\n");
    }

    if(strcmp(arq, "") != 0){
        fclose(f);
    }

}

void calcular_FO(Solucao& s){
  s.fo = N;
  
  for(int i = 0; i < N; i++){
    int id = (i * M) + s.vet_sol[i];
        for(int j = 0; j < vet_qtd_conf[id]; j++){
            int id2 = mat_conf[id][j];
            int pt = id2 / M;
            int pos = id2 % M;
            if(s.vet_sol[pt] == pos){
                s.fo--;
                break;
            }


        }
  }

}

void escrever_sol(char*arq, Solucao& s){
    FILE* f;
    if(strcmp(arq, "") == 0){
        f = stdout;
    }
    else{
        f = fopen(arq, "w");
    }

    fprintf(f, "FO: %d", s.fo);
    for(int i = 0; i < N; i++){
        fprintf(f, "Conflitos do pontos %d: %d", i, vet_qtd_conf[i]);
    }

}

void heu_con_ale(Solucao& s){
    for(int i = 0; i < N; i++)
        s.vet_sol[i] = rand() % M;
}

void heu_con_gul(Solucao& s){
    // Inicializa todos os pontos como não posicionados (-1)
    for(int i = 0; i < N; i++){
        s.vet_sol[i] = -1;
    }

    for(int i = 0; i < N; i++){
        // Padrão: caso nenhuma posição seja 100% livre,
        // assume a posição 0 e deixa a penalização para a calcular_FO
        s.vet_sol[i] = 0;

        for(int p = 0; p < M; p++){
            int id = (i * M) + p;
            int tem_conflito = 0;

            // Verifica se a posição p conflita com algum ponto já colocado
            for(int j = 0; j < vet_qtd_conf[id]; j++){
                int id2 = mat_conf[id][j];
                int pt = id2 / M;
                int pos_viz = id2 % M;

                // Conflita com um ponto que já foi rotulado?
                if(s.vet_sol[pt] == pos_viz){
                    tem_conflito = 1;
                    break;
                }
            }

            // Exatamente como no PMM: se não viola restrição, aloca e para (break)
            if(!tem_conflito){
                s.vet_sol[i] = p;
                break;
            }
        }
    }
}

int main(){

    char arq[50];
    strcpy(arq, "../inst1.txt");
    ler_dados(arq);
    strcpy(arq, "testeCP.txt");
    testar_dados(arq);

    return 0;
}