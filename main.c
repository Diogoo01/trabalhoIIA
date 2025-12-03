#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "algoritmo.h"
#include "funcao.h"

#define DEFAULT_RUNS 10
#define DEFAULT_ITER 10000   // nº de iterações do trepa-colinas

int main(int argc, char *argv[])
{
    char nome_fich[100];
    int C, m;
    int runs, k;
    double *mat;
    int *sol, *best_sol;
    double custo, best_custo = 0.0, mbf = 0.0;

    if (argc >= 2)
    {
        strcpy(nome_fich, argv[1]);
        runs = (argc >= 3) ? atoi(argv[2]) : DEFAULT_RUNS;
    }
    else
    {
        runs = DEFAULT_RUNS;
        printf("Nome do ficheiro da instancia: ");
        gets(nome_fich);
    }

    if (runs <= 0)
        return 0;

    init_rand();

    // lê dados
    mat = init_dados(nome_fich, &C, &m);

    sol = malloc(sizeof(int) * C);
    best_sol = malloc(sizeof(int) * C);
    if (!sol || !best_sol)
    {
        printf("Erro na alocacao de memória das soluções.\n");
        exit(1);
    }

    for (k = 0; k < runs; k++)
    {
        printf("\n=== Repeticao %d ===\n", k + 1);

        // solução inicial
        gera_sol_inicial(sol, C, m);
        escreve_sol(sol, C);

        // trepa colinas
        custo = trepa_colinas(sol, mat, C, m, DEFAULT_ITER);

        printf("Solucao final: ");
        escreve_sol(sol, C);
        printf("Distancia media: %.4f\n", custo);

        mbf += custo;

        if (k == 0 || custo > best_custo)
        {
            best_custo = custo;
            for (int i = 0; i < C; i++)
                best_sol[i] = sol[i];
        }
    }

    printf("\n\nMBF (media das repeticoes): %.4f\n", mbf / runs);
    printf("Melhor solucao encontrada:\n");
    escreve_sol(best_sol, C);
    printf("Melhor distancia media: %.4f\n", best_custo);

    free(mat);
    free(sol);
    free(best_sol);

    return 0;
}
