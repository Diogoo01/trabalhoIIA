#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"

// Inicializa o gerador de números aleatórios
void init_rand()
{
    srand((unsigned) time(NULL));
}

// Inteiro aleatório uniforme [min, max]
int random_l_h(int min, int max)
{
    return min + rand() % (max - min + 1);
}

// Real aleatório [0,1]
float rand_01()
{
    return ((float) rand()) / RAND_MAX;
}

// Lê ficheiro da instância
double* init_dados(char *nome, int *C, int *m)
{
    FILE *f = fopen(nome, "r");
    if (!f)
    {
        printf("Erro a abrir ficheiro %s\n", nome);
        exit(1);
    }

    // primeira linha: C m
    if (fscanf(f, " %d %d", C, m) != 2)
    {
        printf("Formato inválido na primeira linha.\n");
        exit(1);
    }

    // aloca matriz C x C de doubles
    double *mat = malloc(sizeof(double) * (*C) * (*C));
    if (!mat)
    {
        printf("Erro na alocacao de memoria da matriz.\n");
        exit(1);
    }

    // lê as distâncias (assumimos matriz completa C linhas, C colunas)
    for (int i = 0; i < *C; i++)
    {
        for (int j = 0; j < *C; j++)
        {
            if (fscanf(f, " %lf", &mat[i * (*C) + j]) != 1)
            {
                printf("Erro a ler distancia [%d,%d]\n", i, j);
                exit(1);
            }
        }
    }

    fclose(f);
    return mat;
}
