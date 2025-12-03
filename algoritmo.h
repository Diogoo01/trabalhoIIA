#ifndef ALGORITMO_H
#define ALGORITMO_H

// gera solução inicial com exactamente m pontos a 1
void gera_sol_inicial(int *sol, int C, int m);

// escreve solução (índices escolhidos)
void escreve_sol(int *sol, int C);

// trepa-colinas (maximização da distância média)
double trepa_colinas(int sol[], double *mat, int C, int m, int num_iter);

#endif
