#ifndef EVOLUTIVO_H
#define EVOLUTIVO_H

#include "utils.h"
#include "funcao.h"
#include "algoritmo.h"

#ifndef POP_SIZE
#define POP_SIZE 50
#endif

#ifndef NUM_GEN
#define NUM_GEN 200
#endif

#ifndef TORNEIO
#define TORNEIO 2
#endif

#ifndef PX
#define PX 0.7
#endif

#ifndef PM
#define PM 0.1
#endif

extern int usa_hibrido1;
extern int usa_hibrido2;

typedef struct {
    int    *sol;
    double  fitness;
} Individuo;

void repara(int sol[], int C, int m);
void copia(int *dest, const int *orig, int C);

int  seleciona_torneio(const Individuo *pop, int pop_size, int tsize);
void crossover_1ponto(const int *p1, const int *p2, int *f1, int *f2, int C, int m);
void mutacao_swap(int sol[], int C, int m, double pm);

double evolutivo(const double *mat, int C, int m, int *best_sol);

#endif
