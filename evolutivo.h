#ifndef EVOLUTIVO_H
#define EVOLUTIVO_H

#include <stddef.h>

/* Parâmetros (podes ajustar no .c ou passar por defines antes de incluir) */
#ifndef POP_SIZE
#define POP_SIZE 50
#endif

// flags vindas do hibrido.c
extern int usa_hibrido1;
extern int usa_hibrido2;


#ifndef NUM_GEN
#define NUM_GEN  200
#endif

#ifndef PX
#define PX       0.8     /* prob. crossover */
#endif

#ifndef PM
#define PM       0.1     /* prob. mutação */
#endif

#ifndef TORNEIO
#define TORNEIO  3
#endif

#ifndef BIG_NEG
#define BIG_NEG (-1e9)
#endif

/* Se quiseres a tua métrica antiga (soma/m), mete 1 */
#ifndef USE_AVG_DIV_M
#define USE_AVG_DIV_M 0
#endif

typedef struct {
    int    *sol;     /* vetor binário de tamanho C */
    double  fitness;
} Individuo;

/* Utils RNG */
void   init_rand(void);
int    random_l_h(int min, int max);
double rand_01(void);

/* Leitura dados (matriz CxC em memória linear) */
double* init_dados(const char *nome, int *C, int *m);

/* Avaliação / operadores */
double calcula_fit(const int sol[], const double *mat, int C, int m);
void   gera_sol_inicial(int sol[], int C, int m);
void   repara(int sol[], int C, int m);
void   copia(int *dest, const int *orig, int C);

int    seleciona_torneio(const Individuo *pop, int pop_size, int tsize);
void   crossover_1ponto(const int *p1, const int *p2, int *f1, int *f2, int C, int m);
void   mutacao_swap(int sol[], int C, int m, double pm);

/* Algoritmo Evolutivo (devolve melhor fitness e escreve melhor solução em best_sol) */
double evolutivo(const double *mat, int C, int m, int *best_sol);

/* Output */
void escreve_sol(const int *sol, int C);

#endif
