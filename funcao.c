#include "funcao.h"

// Calcula a distância média DM(P)
// sol : vetor de 0/1 com tamanho C
// mat : matriz de distâncias 
// C   : número total de pontos
// m   : número de pontos que DEVEM estar seleccionados
double calcula_fit(int sol[], double *mat, int C, int m)
{
    int selecionados = 0;
    double soma = 0.0;

    // conta quantos estão a 1
    for (int i = 0; i < C; i++)
        if (sol[i] == 1)
            selecionados++;

    // se não tiver exactamente m, devolve um valor mau (para maximização)
    if (selecionados != m || m <= 1)
        return -1e9; // grande negativo

    int pares = 0;

    for (int i = 0; i < C - 1; i++)
    {
        if (sol[i] == 1)
        {
            for (int j = i + 1; j < C; j++)
            {
                if (sol[j] == 1)
                {
                    soma += mat[i * C + j];
                    pares++;
                }
            }
        }
    }

    // fórmula do enunciado: (1/m) * soma_{i<j} dist(ei, ej)
    if (pares == 0)
        return 0.0;

    return soma / m;
}
