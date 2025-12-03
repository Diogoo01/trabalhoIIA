#include <stdio.h>
#include <stdlib.h>
#include "algoritmo.h"
#include "utils.h"
#include "funcao.h"

// Gera solução inicial com exactamente m = nº de 1s
void gera_sol_inicial(int *sol, int C, int m)
{
    for (int i = 0; i < C; i++)
        sol[i] = 0;

    for (int k = 0; k < m; k++)
    {
        int x;
        do {
            x = random_l_h(0, C - 1);
        } while (sol[x] == 1);

        sol[x] = 1;
    }
}

// Escreve índices escolhidos
void escreve_sol(int *sol, int C)
{
    printf("Locais escolhidos: ");
    for (int i = 0; i < C; i++)
        if (sol[i] == 1)
            printf("%d ", i);
    printf("\n");
}

// Gera vizinho trocando um ponto escolhido com um não escolhido
static void gera_vizinho(int a[], int b[], int C)
{
    // copia
    for (int i = 0; i < C; i++)
        b[i] = a[i];

    int p1, p2;

    // posição com 1
    do {
        p1 = random_l_h(0, C - 1);
    } while (b[p1] != 1);

    // posição com 0
    do {
        p2 = random_l_h(0, C - 1);
    } while (b[p2] != 0);

    // troca → continua com o mesmo número de 1s
    b[p1] = 0;
    b[p2] = 1;
}


// Gera vizinho 2: troca DOIS pontos (mantém m fixo)
void gera_vizinho2(int a[], int b[], int C)
{
    int p1, p2, p3, p4;

    // copiar solução
    for (int i = 0; i < C; i++)
        b[i] = a[i];

    // primeira posição com 1
    do {
        p1 = random_l_h(0, C - 1);
    } while (b[p1] != 1);

    // primeira posição com 0
    do {
        p2 = random_l_h(0, C - 1);
    } while (b[p2] != 0);

    // segunda posição com 1 (diferente da primeira)
    do {
        p3 = random_l_h(0, C - 1);
    } while (b[p3] != 1 || p3 == p1);

    // segunda posição com 0 (diferente da primeira)
    do {
        p4 = random_l_h(0, C - 1);
    } while (b[p4] != 0 || p4 == p2);

    // fazer as duas trocas
    b[p1] = 0;
    b[p2] = 1;

    b[p3] = 0;
    b[p4] = 1;
}


// Trepa-colinas (first-choice) – problema de MAXIMIZAÇÃO
double trepa_colinas(int sol[], double *mat, int C, int m, int num_iter)
{
    int *nova_sol = malloc(sizeof(int) * C);
    if (!nova_sol)
    {
        printf("Erro na alocacao de memoria (vizinho)\n");
        exit(1);
    }

    double custo = calcula_fit(sol, mat, C, m);

    for (int i = 0; i < num_iter; i++)
    {
        // gera 1 vizinho
        //gera_vizinho(sol, nova_sol, C);

        gera_vizinho2(sol, nova_sol, C);

        double custo_viz = calcula_fit(nova_sol, mat, C, m);

        // como estamos a MAXIMIZAR, só aceitamos se for melhor ou igual
        if (custo_viz >= custo)
        {
            for (int j = 0; j < C; j++)
                sol[j] = nova_sol[j];
            custo = custo_viz;
        }
    }

    free(nova_sol);
    return custo;
}
