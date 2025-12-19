#include "funcao.h"

double calcula_fit(int sol[], const double *mat, int C, int m)
{
    int selecionados = 0;
    double soma = 0.0;
    int pares = 0;

    for (int i = 0; i < C; i++)
        if (sol[i] == 1)
            selecionados++;

    if (selecionados != m || m <= 1)
        return 0.0;

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

    if (pares == 0)
        return 0.0;

    /* (1/m) * sum_{i<j} dist */
    return soma / (double)m;
}
