#define _CRT_SECURE_NO_WARNINGS 1
#include "evolutivo.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/* ================= RNG ================= */

void init_rand(void) {
    srand((unsigned)time(NULL));
}

int random_l_h(int min, int max) {
    return min + rand() % (max - min + 1);
}

double rand_01(void) {
    return (double)rand() / (double)RAND_MAX;
}

/* ================= IO ================= */

double* init_dados(const char *nome, int *C, int *m)
{
    FILE *f = fopen(nome, "r");
    if (!f) {
        printf("Erro a abrir ficheiro %s\n", nome);
        exit(1);
    }

    if (fscanf(f, " %d %d", C, m) != 2) {
        printf("Erro na primeira linha (C m)\n");
        exit(1);
    }

    /* matriz CxC (linear) */
    double *mat = (double*)malloc(sizeof(double) * (size_t)(*C) * (size_t)(*C));
    if (!mat) {
        printf("Erro malloc matriz\n");
        exit(1);
    }

    for (int i = 0; i < *C; i++)
        for (int j = 0; j < *C; j++)
            mat[i * (*C) + j] = 0.0;

    /* Formato: e1 e2 50.2 (como no teu original) */
    char a[32], b[32];
    double dist;

    while (fscanf(f, " %31s %31s %lf", a, b, &dist) == 3) {
        int ia = atoi(a + 1) - 1;
        int ib = atoi(b + 1) - 1;

        if (ia >= 0 && ia < *C && ib >= 0 && ib < *C) {
            mat[ia * (*C) + ib] = dist;
            mat[ib * (*C) + ia] = dist;
        }
    }

    fclose(f);
    return mat;
}

/* ================= Avaliação ================= */

double calcula_fit(const int sol[], const double *mat, int C, int m)
{
    int sel = 0;
    for (int i = 0; i < C; i++)
        if (sol[i] == 1) sel++;

    if (sel != m || m <= 1)
        return BIG_NEG;

    double soma = 0.0;
    for (int i = 0; i < C - 1; i++) {
        if (!sol[i]) continue;
        for (int j = i + 1; j < C; j++) {
            if (sol[j])
                soma += mat[i * C + j];
        }
    }

#if USE_AVG_DIV_M
    return soma / (double)m; /* opção antiga */
#else
    /* média por par */
    return soma / ((double)m * (double)(m - 1) / 2.0);
#endif
}

/* ================= Helpers ================= */

void gera_sol_inicial(int sol[], int C, int m)
{
    for (int i = 0; i < C; i++) sol[i] = 0;

    int count = 0;
    while (count < m) {
        int p = random_l_h(0, C - 1);
        if (!sol[p]) {
            sol[p] = 1;
            count++;
        }
    }
}

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

void copia(int *dest, const int *orig, int C) {
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

void crossover_1ponto(const int *p1, const int *p2, int *f1, int *f2, int C, int m)
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
    (void)m; /* swap mantém o nº de 1s, não precisa de m */

    if (rand_01() < pm) {
        int i, j;
        /* garante que escolhe um 0 e um 1 */
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


    // HÍBRIDO 1 — trepa-colinas na população inicial
    if (usa_hibrido1) {
    for (int i = 0; i < POP_SIZE; i++) {
        pop[i].fitness = trepa_colinas(
            pop[i].sol, (double *)mat, C, m, 1000
        );
    }
    }


    double best = pop[0].fitness;
    copia(best_sol, pop[0].sol, C);

    for (int g = 0; g < NUM_GEN; g++)
    {
        /* 1) elitismo (nova[0]) */
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

        /* 2) gerar restantes indivíduos sem nunca escrever em nova[0] */
        int i = 1;
        while (i < POP_SIZE)
        {
            int p1 = seleciona_torneio(pop, POP_SIZE, TORNEIO);
            int p2;
            do { p2 = seleciona_torneio(pop, POP_SIZE, TORNEIO); } while (p2 == p1); /* opcional */

            if (i == POP_SIZE - 1) {
                /* caso POP_SIZE seja ímpar: gera só 1 filho */
                copia(nova[i].sol, pop[p1].sol, C);

                if (rand_01() < PX) {
                    /* aplica crossover “parcial”: mistura com p2 e repara */
                    int ponto = random_l_h(1, C - 1);
                    for (int k = ponto; k < C; k++)
                        nova[i].sol[k] = pop[p2].sol[k];
                    repara(nova[i].sol, C, m);
                }

                mutacao_swap(nova[i].sol, C, m, PM);
                nova[i].fitness = calcula_fit(nova[i].sol, mat, C, m);
                i += 1;
            } else {
                /* caso normal: 2 filhos */
                if (rand_01() < PX) {
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

        /* 3) copiar nova -> pop */
        for (int k = 0; k < POP_SIZE; k++) {
            copia(pop[k].sol, nova[k].sol, C);
            pop[k].fitness = nova[k].fitness;
        }



        /* 4) HÍBRIDO 2 (memético): aplicar trepa-colinas a X% da população */
        if (usa_hibrido2) {
        int limite = (int)(POP_SIZE * 0.1);  // 10%
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

/* ================= Output ================= */

void escreve_sol(const int *sol, int C)
{
    printf("Locais escolhidos: ");
    for (int i = 0; i < C; i++)
        if (sol[i]) printf("%d ", i);
    printf("\n");
}

/* ================= Main opcional ================= */
/* Compila com -DBUILD_MAIN para incluir main neste ficheiro */
#ifdef BUILD_MAIN
int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Uso: %s ficheiro_instancia.txt\n", argv[0]);
        return 0;
    }

    int C, m;
    init_rand();

    double *mat = init_dados(argv[1], &C, &m);

    int *best_sol = (int*)malloc(sizeof(int) * (size_t)C);
    if (!best_sol) {
        printf("Erro malloc best_sol\n");
        exit(1);
    }

    double best = evolutivo(mat, C, m, best_sol);

    printf("\n=== Melhor solucao (Evolutivo) ===\n");
    escreve_sol(best_sol, C);
    printf("Distancia media: %.4f\n", best);

    free(best_sol);
    free(mat);
    return 0;
}
#endif
