#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "darp.h"


void ler_dados(char* arq){
    FILE* f = fopen(arq, "r");

    fscanf(f,"%d %d %d %d %d", &veiculos, &req, &dur_max_rota, &tempo_max_viagem, &tempo_max_espera);

    locais = (req * 2) + 2;

    for(int i = 0; i < veiculos; i++){
        fscanf(f, "%d", &vet_cap_vei[i]);
    }
    
    for(int i = 0; i < locais; i++){
        fscanf(f,"%d", &vet_num_ass_loc[i]);
    }

    for(int i = 0; i < locais; i++){
        fscanf(f,"%d", &vet_tempo_servico[i]);
    }

    for(int i = 0; i < locais; i++){
        fscanf(f,"%d", &vet_ini_jan_tempo[i]);
    }

    for(int i = 0; i < locais; i++){
        fscanf(f,"%d", &vet_fin_jan_tempo[i]);
    }

    for(int i = 0; i < locais; i++){
        for(int j = 0; j < locais; j++){
            fscanf(f, "%d", &mat_tempo_desloc[i][j]);
        }
    }

    fclose(f);
}

void testar_dados(char* arq){
    FILE* f;
    if(strcmp(arq, "") == 0){
        f = stdout;
    }
    else{
        f = fopen(arq, "w");
    

    fprintf(f,"%d %d %d %d %d\n\n", veiculos, req, dur_max_rota, tempo_max_viagem, tempo_max_espera);

    locais = (req * 2) + 2;

    for(int i = 0; i < veiculos; i++){
        fprintf(f, "%d\t", vet_cap_vei[i]);
    }
    fprintf(f,"\n\n");

    for(int i = 0; i < locais; i++){
        fprintf(f,"%d\t", vet_num_ass_loc[i]);
    }
    fprintf(f,"\n\n");

    for(int i = 0; i < locais; i++){
        fprintf(f,"%d\t", vet_tempo_servico[i]);
    }
    fprintf(f,"\n\n");

    for(int i = 0; i < locais; i++){
        fprintf(f,"%d\t", vet_ini_jan_tempo[i]);
    }
    fprintf(f,"\n");

    for(int i = 0; i < locais; i++){
        fprintf(f,"%d\t", vet_fin_jan_tempo[i]);
    }
    fprintf(f,"\n\n");

    for(int i = 0; i < locais; i++){
        for(int j = 0; j < locais; j++){
            fprintf(f, "%d\t", mat_tempo_desloc[i][j]);
        }
        fprintf(f,"\n");
    }

    fclose(f);
    }
}

void escrever_sol(Solucao& s){
    printf("FO: %d", s.fo);
    for(int i = 0; i < locais; i++){
        for(int j = 0; j < locais; j++){
            printf("%d\t", mat_tempo_desloc[i][j]);
        }
        printf("\n");
    }
}

void calcular_FO(Solucao& s){
    s.fo = 0;
    //FO = 3 * nVeiculos utilizados + 1 * distancia de um local a outro
    
}

int main(){
    
    char arq[50];
    strcpy(arq, "../darp1.txt");
    ler_dados(arq);
    strcpy(arq, "testedarp.txt");
    testar_dados(arq);



    return 0;
}