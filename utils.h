#ifndef UTILS_H
#define UTILS_H

void init_rand(void);
int  random_l_h(int min, int max);
float rand_01(void);

/*
 * Lê instância no formato:
 *   C m
 *   eX eY dist
 * Devolve matriz CxC (linearizada).
 */
double* init_dados(char *nome, int *C, int *m);

#endif
