#ifndef UTILS_H
#define UTILS_H

void init_rand();
int random_l_h(int min, int max);
float rand_01();

// Lê o ficheiro da instância
// Devolve a matriz de distâncias 
// Preenche C (número de pontos) e m (pontos a escolher)
double* init_dados(char *nome, int *C, int *m);

#endif
