#include <stdio.h>
#include <stdlib.h>

#include "hibrido.h"
#include "evolutivo.h"
#include "algoritmo.h"
#include "utils.h"

#define HIBRIDO1 1
#define HIBRIDO2 2

int usa_hibrido1 = 0;
int usa_hibrido2 = 0;

double hibrido(const double *mat, int C, int m, int *best_sol, int tipo)
{
    usa_hibrido1 = 0;
    usa_hibrido2 = 0;

    if (tipo == HIBRIDO1)
        usa_hibrido1 = 1;
    else if (tipo == HIBRIDO2)
        usa_hibrido2 = 1;

    double best = evolutivo(mat, C, m, best_sol);

    usa_hibrido1 = 0;
    usa_hibrido2 = 0;

    return best;
}
