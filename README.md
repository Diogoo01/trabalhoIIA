🧠 Trabalho Prático 2 – Introdução à Inteligência Artificial
Problema de Otimização: Roteiro Turístico Sustentável

Este repositório contém a implementação do Trabalho Prático nº 2 da UC Introdução à Inteligência Artificial.
O objetivo é desenvolver e comparar métodos de otimização aplicados à seleção de m pontos turísticos a partir de C candidaturas, maximizando a distância média entre os locais escolhidos.

📌 Descrição do Problema

A Câmara Municipal pretende criar um roteiro turístico equilibrado e sustentável, evitando a concentração de visitantes.
Para isso, é necessário escolher m locais (de um conjunto de C) que estejam o mais afastados possível.

Cada instância contém:

Número total de locais (C)

Número de locais a selecionar (m)

Matriz de distâncias entre todos os pontos

Uma solução é válida apenas se selecionar exatamente m locais.
O objetivo é maximizar a distância média entre todos os pares de pontos escolhidos.

⚙️ Algoritmos Implementados

O trabalho inclui três abordagens:

1️⃣ Pesquisa Local

Trepa-Colinas / Recristalização Simulada

Múltiplas vizinhanças

Estratégias de penalização ou reparação de soluções inválidas

2️⃣ Algoritmo Evolutivo

Representação binária

Vários tipos de recombinação e mutação

Dois métodos de seleção

Tratamento de soluções inválidas

3️⃣ Métodos Híbridos

Combinação entre o melhor algoritmo evolutivo e o melhor de pesquisa local

Duas variações híbridas testadas

🔬 Estudo Experimental

Para as diferentes instâncias fornecidas foram analisados:

Número de iterações

Parâmetros dos algoritmos

População, mutação, recombinação, seleção

Comparação entre:

Melhor pesquisa local

Melhor evolutivo

Híbrido 1

Híbrido 2

Cada experiência é repetida 10 vezes, sendo analisado:

Melhor valor obtido

Média das execuções

📁 Estrutura do Repositório

Código fonte em C

Funções de avaliação e operadores genéticos

Ficheiros de instâncias para teste

Resultados experimentais e relatório (a adicionar)
