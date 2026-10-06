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

        if(mat_tempo_atendimento[k][navio] == 0)
         s.fo += PESO_INCOMPATIVEL;

        if(tempo > tempolimitesaida[navio]){
            s.fo += PESO_TEMPOLIMITE_NAVIO * MAX(0, tempo - tempolimitesaida[navio]);
            }
        }

        if(tempo > fechamento_berco[k]){
            s.fo += PESO_TEMPOLIMITE_BERCO * MAX(0, tempo - fechamento_berco[k]);
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


void ordenar_objetos()
{
    for(int j = 0; j < navios; j++)
        vet_ind_ord_obj[j] = j;
    
    int flag = 1;
    while(flag)
    {
        flag = 0;
           for(int j = 0; j < navios - 1; j++)
            {
                if((double)tempochegada[vet_ind_ord_obj[j+1]] < (double)tempochegada[vet_ind_ord_obj[j]])
                {
                    int aux = vet_ind_ord_obj[j];
                    vet_ind_ord_obj[j] = vet_ind_ord_obj[j+1];
                    vet_ind_ord_obj[j+1] = aux;
                    flag = 1;
                }
            }
    }
}

void heu_con_ale(SolucaoPAB& s){
    memset(s.qtd_berco, 0, sizeof(s.qtd_berco));
    int berco;
    for(int j = 0; j < navios; j++){
        while(mat_tempo_atendimento[berco][j] == 0){
            berco = rand() % bercos;              // sorteia o berço do navio j
        }
        s.mat_seq_bercos[berco][s.qtd_berco[berco]] = j;
        s.qtd_berco[berco]++;
    }
}

void heu_con_gul(SolucaoPAB& s){
    memset(s.mat_seq_bercos, 0, sizeof(s.mat_seq_bercos));
    memset(s.qtd_berco, 0, sizeof(s.qtd_berco));
    ordenar_objetos();
    int berco = 0;
    for(int j = 0; j < navios; j++){
        while(mat_tempo_atendimento[berco][vet_ind_ord_obj[j]] == 0){
            berco = (berco + 1) % bercos;
        }
        s.mat_seq_bercos[berco][s.qtd_berco[berco]] = vet_ind_ord_obj[j];
        s.qtd_berco[berco]++;
        berco++;
        if(berco == bercos)
            berco = 0;
    }
}

void heu_con_ale_gul(SolucaoPAB& s, const double per_ale){
    int vet_aux[MAX_NAVIOS];
    ordenar_objetos();
    memcpy(vet_aux, vet_ind_ord_obj, sizeof(vet_ind_ord_obj));
    int qtde = MAX(1, (per_ale / 100) * navios);

    for(int i = 0; i < qtde; i++){
        int pos = i + rand() % (navios - i);
        int aux = vet_aux[i];
        vet_aux[i] = vet_aux[pos];
        vet_aux[pos] = aux;
    }

    memset(s.mat_seq_bercos, 0, sizeof(s.mat_seq_bercos));
    memset(s.qtd_berco, 0, sizeof(s.qtd_berco));
    int berco = 0;
    for(int j = 0; j < navios; j++){
        while(mat_tempo_atendimento[berco][vet_aux[j]] == 0){
            berco = (berco + 1) % bercos;
        }
        s.mat_seq_bercos[berco][s.qtd_berco[berco]] = vet_aux[j];
        s.qtd_berco[berco]++;
        berco++;
        if(berco == bercos)
            berco = 0;
    }

}

int main (){

    srand(time(NULL));   // semente diferente a cada execucao

    char arq[50];

    // tenta caminhos comuns (a IDE pode executar a partir de outra pasta)
    const char* caminhos[] = {"i01.txt", "../i01.txt", "PAB/i01.txt", "../PAB/i01.txt"};
    int achou = 0;
    for(int c = 0; c < 4; c++){
        FILE* teste = fopen(caminhos[c], "r");
        if(teste != NULL){
            fclose(teste);
            strcpy(arq, caminhos[c]);
            achou = 1;
            break;
        }
    }
    if(!achou){
        printf("i01.txt nao encontrado. Coloque o arquivo na pasta de onde o programa e executado.\n");
        return 1;
    }
    ler_dados(arq);
    strcpy(arq, "teste1.txt");
    testar_dados(arq);

SolucaoPAB sol;

memset(sol.mat_seq_bercos, -1, sizeof(sol.mat_seq_bercos));
memset(sol.qtd_berco, 0, sizeof(sol.qtd_berco));

    heu_con_ale_gul(sol, 0);
    calcularFO(sol);

    strcpy(arq, "");
    escrever_sol(arq, sol);


    return 0;
}