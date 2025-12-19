#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"

/* ================= RNG ================= */

void init_rand(void)
{
    srand((unsigned)time(NULL));
}

int random_l_h(int min, int max)
{
    return min + rand() % (max - min + 1);
}

float rand_01(void)
{
    return (float)rand() / (float)RAND_MAX;
}

/* ================= IO ================= */

double* init_dados(char *nome, int *C, int *m)
{
    FILE *f = fopen(nome, "r");
    if (!f) {
        printf("Erro a abrir ficheiro %s\n", nome);
        exit(1);
    }

    if (fscanf(f, " %d %d", C, m) != 2) {
        printf("Formato inválido na primeira linha.\n");
        fclose(f);
        exit(1);
    }

    double *mat = (double*)malloc(sizeof(double) * (size_t)(*C) * (size_t)(*C));
    if (!mat) {
        printf("Erro na alocacao de memoria da matriz.\n");
        fclose(f);
        exit(1);
    }

    for (int i = 0; i < *C; i++)
        for (int j = 0; j < *C; j++)
            mat[i * (*C) + j] = 0.0;

    /* Formato: e1 e2 50.2 */
    char a[32], b[32];
    double dist;

    while (fscanf(f, " %31s %31s %lf", a, b, &dist) == 3) {
        int ia = atoi(a + 1) - 1;  /* "e1" -> 0 */
        int ib = atoi(b + 1) - 1;

        if (ia >= 0 && ia < *C && ib >= 0 && ib < *C) {
            mat[ia * (*C) + ib] = dist;
            mat[ib * (*C) + ia] = dist;
        }
    }

    fclose(f);
    return mat;
}
