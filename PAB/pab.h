#define MAX_NAVIOS 100
#define MAX_BERCOS 20

#define PESO_TEMPOLIMITE_NAVIO 100
#define PESO_TEMPOLIMITE_BERCO 100
#define PESO_INCOMPATIVEL 100

typedef struct tSolucaoBIN{
    int fo;
    int mat_seq_bercos[MAX_BERCOS][MAX_NAVIOS];
    int qtd_berco[MAX_NAVIOS];

}SolucaoPAB;

int navios;
int bercos;
int tempolimitesaida[MAX_NAVIOS];
int tempochegada[MAX_NAVIOS];
int abertura_berco[MAX_BERCOS];
int fechamento_berco[MAX_BERCOS];
int mat_tempo_atendimento[MAX_BERCOS][MAX_NAVIOS];

int vet_ind_ord_obj[MAX_NAVIOS];

void testar_dados(char *arq);
void ler_dados(char* arq);
void calcularFO(SolucaoPAB& s);
void escrever_sol(SolucaoPAB& s, char* arq);

void ordenar_objetos();
void heu_ale_gul(SolucaoPAB& s);
void heu_con_ale_gul(SolucaoPAB& s, const double per_ale);
void heu_con_gul(SolucaoPAB& s);
void gerar_vizinha(SolucaoPAB& s);