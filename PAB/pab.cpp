#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <memory.h>
#include "pab.h"

#define MAX(X, Y) (((X>Y) ? X : Y))

void ler_dados(char* arq){

    FILE* f = fopen(arq, "r");

    if(f == NULL)
    {
    printf("Erro ao abrir o arquivo!\n");
    exit(1);
    }

   fscanf(f, "%d %d", &navios, &bercos);

    for(int i = 0; i < bercos; i++){
        for(int j = 0; j < navios; j++){
            fscanf(f, "%d", &mat_tempo_atendimento[i][j]);
        }
    }

    printf("\n");

    for(int i = 0; i <bercos; i++){
        fscanf(f, "%d %d", &abertura_berco[i], &fechamento_berco[i]);
    }

    for(int i = 0; i < navios; i++){
        fscanf(f, "%d " , &tempochegada[i]);
    }

for(int i = 0; i < navios; i++){
        fscanf(f, "%d ", &tempolimitesaida[i]);
    }

    fclose(f);
}

void testar_dados(char*arq){
    FILE* f = fopen(arq, "w");
 

     fprintf(f, "%d %d\n", navios, bercos);
    for(int i = 0; i < bercos; i++){
        for(int j = 0; j < navios; j++){
            fprintf(f, "%d ", mat_tempo_atendimento[i][j]);
        }
        fprintf(f,"\n");
    }

    for(int i = 0; i <bercos; i++){
        fprintf(f, "%d %d\n", abertura_berco[i], fechamento_berco[i]);
    }

    for(int i = 0; i < navios; i++){
        fprintf(f, "%d " , tempochegada[i]);
    }

fprintf(f,"\n");

for(int i = 0; i < navios; i++){
        fprintf(f, "%d ", tempolimitesaida[i]);
    }

    fclose(f);
}

void calcularFO(SolucaoPAB& s){
    s.fo = 0;

    for(int k =0; k < bercos; k++){
      int tempo = abertura_berco[k];
        for(int j = 0; j < s.qtd_berco[k]; j++){
        int navio = s.mat_seq_bercos[k][j];
        if(tempo < tempochegada[navio]){
            tempo = tempochegada[navio];
        }
        s.fo += tempo - tempochegada[navio];
        tempo += mat_tempo_atendimento[k][navio];

        if(tempo > tempolimitesaida[navio]){
            s.fo += PESO_TEMPOLIMITE_NAVIO * MAX(0, tempo - tempolimitesaida[navio]);
        }
        }

        if(tempo > fechamento_berco[k]){
            PESO_TEMPOLIMITE_BERCO * MAX(0, tempo - fechamento_berco[k]);
        }
    }
}

void escrever_sol(char* arq, SolucaoPAB& s){
    FILE* f;
    if(strcmp(arq, "") == 0){
        f = stdout;
    }
    else{
        f = fopen(arq, "w");
    }

    fprintf(f, "FO: %d\n", s.fo);
    for(int i  = 0; i < bercos; i++){
        fprintf(f, "Berco: %d\n", i + 1);
        for(int j = 0; j < s.qtd_berco[i]; j++){
            fprintf(f, "%d ", s.mat_seq_bercos[i][j]);
        }
        fprintf(f, "\n");
    }

    if(strcmp(arq, "") != 0){
        fclose(f);
    }


}

int main (){

    char arq[50];
    strcpy(arq, "../i01.txt");
    ler_dados(arq);
    strcpy(arq, "teste1.txt");
    testar_dados(arq);

SolucaoPAB sol;

memset(sol.mat_seq_bercos, -1, sizeof(sol.mat_seq_bercos));
memset(sol.qtd_berco, 0, sizeof(sol.qtd_berco));

// Berço 0
sol.mat_seq_bercos[0][0] = 0;
sol.mat_seq_bercos[0][1] = 1;
sol.mat_seq_bercos[0][2] = 2;
sol.qtd_berco[0] = 3;

// Berço 1
sol.mat_seq_bercos[1][0] = 3;
sol.mat_seq_bercos[1][1] = 4;
sol.qtd_berco[1] = 2;

// Berço 2
sol.mat_seq_bercos[2][0] = 5;
sol.mat_seq_bercos[2][1] = 6;
sol.qtd_berco[2] = 2;

    calcularFO(sol);

    strcpy(arq, "solucao.txt");
    escrever_sol(arq, sol);

    return 0;
}