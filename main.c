#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "algoritmo.h"   // trepa_colinas, etc
#include "funcao.h"

// Se tiveres estes ficheiros, inclui-os:
#include "evolutivo.h"   // evolutivo(...)
#include "hibrido.h"     // hibrido(...)

#define DEFAULT_RUNS 10
#define DEFAULT_ITER 10000

static int le_int_ate_valido(const char *msg, int min, int max)
{
    char line[64];
    int v;

    do {
        printf("%s", msg);
        if (!fgets(line, sizeof(line), stdin))
            continue;
        if (sscanf(line, "%d", &v) != 1)
            continue;
    } while (v < min || v > max);

    return v;
}

int main(int argc, char *argv[])
{
    char nome_fich[100];
    int C, m;
    int runs, k;
    double *mat;

    // Ler ficheiro + runs (mantive o teu comportamento atual)
    if (argc >= 2)
    {
        strcpy(nome_fich, argv[1]);
        runs = (argc >= 3) ? atoi(argv[2]) : DEFAULT_RUNS;
    }
    else
    {
        runs = DEFAULT_RUNS;
        printf("Nome do ficheiro da instancia: ");
        fgets(nome_fich, sizeof(nome_fich), stdin);
        nome_fich[strcspn(nome_fich, "\n")] = 0;
    }

    if (runs <= 0)
        return 0;

    init_rand();

    // lê dados (matriz de distâncias)
    mat = init_dados(nome_fich, &C, &m);

    // MENU (repete até escolha válida)
    printf("\n=============================\n");
    printf(" TP2 IIA - Menu de Opcoes\n");
    printf("=============================\n");
    printf("1 - Pesquisa Local (Trepa-Colinas)\n");
    printf("2 - Algoritmo Evolutivo\n");
    printf("3 - Algoritmo Hibrido\n\n");

    int opcao = le_int_ate_valido("Escolha (1-3): ", 1, 3);

    // Variáveis comuns
    int *sol = NULL, *best_sol = NULL;
    double custo, best_custo = 0.0, mbf = 0.0;

    // Alocar solução (para todos os modos)
    sol = (int*)malloc(sizeof(int) * C);
    best_sol = (int*)malloc(sizeof(int) * C);
    if (!sol || !best_sol)
    {
        printf("Erro na alocacao de memoria.\n");
        exit(1);
    }

    // Executa o algoritmo escolhido, repetido runs vezes (para MBF)
    for (k = 0; k < runs; k++)
    {
        printf("\n=== Repeticao %d ===\n", k + 1);

        if (opcao == 1)
        {
            // 1) Pesquisa Local
            gera_sol_inicial(sol, C, m);
            custo = trepa_colinas(sol, mat, C, m, DEFAULT_ITER);

            printf("Solucao final: ");
            escreve_sol(sol, C);
            printf("Distancia media: %.4f\n", custo);
        }
        else if (opcao == 2)
        {
            // 2) Evolutivo (devolve best fitness e escreve best_sol)
            custo = evolutivo(mat, C, m, best_sol);

            // para manter output semelhante, copia best_sol para sol
            for (int i = 0; i < C; i++) sol[i] = best_sol[i];

            printf("Melhor solucao (Evolutivo): ");
            escreve_sol(sol, C);
            printf("Distancia media: %.4f\n", custo);
        }
        else
        {
            // 3) Híbrido (sub-menu simples para escolher abordagem)
            printf("\n--- Hibrido ---\n");
            printf("1 - Hibrido 1 (TC na populacao inicial)\n");
            printf("2 - Hibrido 2 (TC em X%% por geracao / memetico)\n");
            int tipo = le_int_ate_valido("Escolha (1-2): ", 1, 2);

            custo = hibrido(mat, C, m, best_sol, tipo);

            for (int i = 0; i < C; i++) sol[i] = best_sol[i];

            printf("Melhor solucao (Hibrido %d): ", tipo);
            escreve_sol(sol, C);
            printf("Distancia media: %.4f\n", custo);
        }

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
