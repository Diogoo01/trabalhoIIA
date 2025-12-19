#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <stdlib.h>

#include "evolutivo.h"

/* ================= Helpers ================= */

void repara(int sol[], int C, int m)
{
    int ones = 0;
    for (int i = 0; i < C; i++)
        if (sol[i]) ones++;

    while (ones > m) {
        int i = random_l_h(0, C - 1);
        if (sol[i]) {
            sol[i] = 0;
            ones--;
        }
    }

    while (ones < m) {
        int i = random_l_h(0, C - 1);
        if (!sol[i]) {
            sol[i] = 1;
            ones++;
        }
    }
}

void copia(int *dest, const int *orig, int C)
{
    for (int i = 0; i < C; i++)
        dest[i] = orig[i];
}

/* ================= Seleção ================= */

int seleciona_torneio(const Individuo *pop, int pop_size, int tsize)
{
    int best = random_l_h(0, pop_size - 1);

    for (int i = 1; i < tsize; i++) {
        int c = random_l_h(0, pop_size - 1);
        if (pop[c].fitness > pop[best].fitness)
            best = c;
    }
    return best;
}

/* ================= Operadores ================= */

void crossover_1ponto(
    const int *p1, const int *p2,
    int *f1, int *f2,
    int C, int m
)
{
    int ponto = random_l_h(1, C - 1);

    for (int i = 0; i < C; i++) {
        if (i < ponto) {
            f1[i] = p1[i];
            f2[i] = p2[i];
        } else {
            f1[i] = p2[i];
            f2[i] = p1[i];
        }
    }

    repara(f1, C, m);
    repara(f2, C, m);
}

void mutacao_swap(int sol[], int C, int m, double pm)
{
    (void)m;

    if ((double)rand_01() < pm) {
        int i, j;
        do {
            i = random_l_h(0, C - 1);
            j = random_l_h(0, C - 1);
        } while (sol[i] == sol[j]);

        int tmp = sol[i];
        sol[i] = sol[j];
        sol[j] = tmp;
    }
}

/* ================= Evolutivo ================= */

double evolutivo(const double *mat, int C, int m, int *best_sol)
{
    Individuo pop[POP_SIZE], nova[POP_SIZE];

    for (int i = 0; i < POP_SIZE; i++) {
        pop[i].sol  = (int*)malloc(sizeof(int) * (size_t)C);
        nova[i].sol = (int*)malloc(sizeof(int) * (size_t)C);

        if (!pop[i].sol || !nova[i].sol) {
            printf("Erro malloc população\n");
            exit(1);
        }

        gera_sol_inicial(pop[i].sol, C, m);
        pop[i].fitness = calcula_fit(pop[i].sol, mat, C, m);
    }

    if (usa_hibrido1) {
        for (int i = 0; i < POP_SIZE; i++) {
            pop[i].fitness = trepa_colinas(pop[i].sol, (double*)mat, C, m, 1000);
        }
    }

    double best = pop[0].fitness;
    copia(best_sol, pop[0].sol, C);

    for (int g = 0; g < NUM_GEN; g++) {

        int best_idx = 0;
        for (int i = 1; i < POP_SIZE; i++)
            if (pop[i].fitness > pop[best_idx].fitness)
                best_idx = i;

        copia(nova[0].sol, pop[best_idx].sol, C);
        nova[0].fitness = pop[best_idx].fitness;

        if (nova[0].fitness > best) {
            best = nova[0].fitness;
            copia(best_sol, nova[0].sol, C);
        }

        int i = 1;
        while (i < POP_SIZE) {
            int p1 = seleciona_torneio(pop, POP_SIZE, TORNEIO);
            int p2;
            do {
                p2 = seleciona_torneio(pop, POP_SIZE, TORNEIO);
            } while (p2 == p1);

            if (i == POP_SIZE - 1) {
                copia(nova[i].sol, pop[p1].sol, C);

                if ((double)rand_01() < PX) {
                    int ponto = random_l_h(1, C - 1);
                    for (int k = ponto; k < C; k++)
                        nova[i].sol[k] = pop[p2].sol[k];
                    repara(nova[i].sol, C, m);
                }

                mutacao_swap(nova[i].sol, C, m, PM);
                nova[i].fitness = calcula_fit(nova[i].sol, mat, C, m);
                i++;
            } else {
                if ((double)rand_01() < PX) {
                    crossover_1ponto(pop[p1].sol, pop[p2].sol,
                                     nova[i].sol, nova[i + 1].sol, C, m);
                } else {
                    copia(nova[i].sol,     pop[p1].sol, C);
                    copia(nova[i + 1].sol, pop[p2].sol, C);
                }

                mutacao_swap(nova[i].sol, C, m, PM);
                nova[i].fitness = calcula_fit(nova[i].sol, mat, C, m);

                mutacao_swap(nova[i + 1].sol, C, m, PM);
                nova[i + 1].fitness = calcula_fit(nova[i + 1].sol, mat, C, m);

                i += 2;
            }
        }

        for (int k = 0; k < POP_SIZE; k++) {
            copia(pop[k].sol, nova[k].sol, C);
            pop[k].fitness = nova[k].fitness;
        }

        if (usa_hibrido2) {
            int limite = (int)(POP_SIZE * 0.1);
            for (int k = 0; k < limite; k++) {
                pop[k].fitness = trepa_colinas(pop[k].sol, (double*)mat, C, m, 1000);
            }
        }
    }

    for (int i2 = 0; i2 < POP_SIZE; i2++) {
        free(pop[i2].sol);
        free(nova[i2].sol);
    }

    return best;
}
