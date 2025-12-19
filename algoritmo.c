#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "algoritmo.h"
#include "utils.h"
#include "funcao.h"

// Gera uma solução inicial aleatória com exatamente m pontos escolhidos
void gera_sol_inicial(int *sol, int C, int m)
{
    // põe tudo a 0
    for (int i = 0; i < C; i++)
        sol[i] = 0;

    // escolhe m posições aleatórias para pôr a 1
    int count = 0;
    while (count < m)
    {
        int pos = random_l_h(0, C - 1);
        if (sol[pos] == 0)
        {
            sol[pos] = 1;
            count++;
        }
    }
}

// Escreve solução (índices dos escolhidos)
void escreve_sol(int *sol, int C)
{
    printf("Locais escolhidos: ");
    for (int i = 0; i < C; i++)
        if (sol[i] == 1)
            printf("%d ", i);
    printf("\n");
}

// Trepa-colinas (maximizar fitness)
double trepa_colinas(int sol[], double *mat, int C, int m, int num_iter)
{
    int *melhor_sol = (int *)malloc(sizeof(int) * C);
    int *nova_sol = (int *)malloc(sizeof(int) * C);

    if (!melhor_sol || !nova_sol)
    {
        printf("Erro de memória no trepa-colinas.\n");
        exit(1);
    }

    // inicializa
    for (int i = 0; i < C; i++)
        melhor_sol[i] = sol[i];

    double melhor_fit = calcula_fit(melhor_sol, mat, C, m);

    for (int iter = 0; iter < num_iter; iter++)
    {
        // copia melhor_sol para nova_sol
        for (int i = 0; i < C; i++)
            nova_sol[i] = melhor_sol[i];

        // tenta swap 1 -> 0 e 0 -> 1 para manter m selecionados
        int i1, i0;
        do
        {
            i1 = random_l_h(0, C - 1);
        } while (nova_sol[i1] == 0);

        do
        {
            i0 = random_l_h(0, C - 1);
        } while (nova_sol[i0] == 1);

        nova_sol[i1] = 0;
        nova_sol[i0] = 1;

        double fit = calcula_fit(nova_sol, mat, C, m);
        if (fit > melhor_fit)
        {
            melhor_fit = fit;
            for (int i = 0; i < C; i++)
                melhor_sol[i] = nova_sol[i];
        }
    }

    // devolve melhor para sol
    for (int i = 0; i < C; i++)
        sol[i] = melhor_sol[i];

    free(melhor_sol);
    free(nova_sol);

    return melhor_fit;
}
