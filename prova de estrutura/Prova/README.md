# Trabalho de Estrutura de Dados

Este repositório tem três programas em C feitos para o trabalho de Estrutura de Dados. Eles simulam um sistema que organiza e consulta dados de sensores.

## Como Compilar
No terminal, entre na pasta do projeto e digite:

gcc -o organizacao p1_organizacao.c
gcc -o consulta p2_consulta.c -lm
gcc -o geracao p3_geracao.c

## Como Executar

Programa 1 – Organização dos Dados

./organizacao dados_brutos.txt
Cria arquivos como SENSOR1.txt, SENSOR2.txt, etc., com dados separados por sensor.

Formato do arquivo de entrada:

<TIMESTAMP> <ID_SENSOR> <VALOR>
Exemplo:

1718300000 TEMP 25.6
Programa 2 – Consulta por Instante

./consulta <id_sensor> <dd> <mm> <aaaa> <hh> <min> <ss>
Exemplo:

./consulta TEMP 15 06 2025 10 30 00
Mostra a leitura mais próxima do horário indicado.

Programa 3 – Geração de Arquivo de Teste

./geracao 01 01 2024 00 00 00 31 12 2024 23 59 59 TEMP inteiro UMID racional LUZ booleano MSG string

Gera um arquivo dados_teste.txt com 2000 leituras por sensor.

Tipos suportados: inteiro, booleano, racional, string.

## Observações
O código usa apenas recursos básicos de C: structs, arquivos, ponteiros e strings.

A busca no Programa 2 usa busca binária para melhor desempenho.

Todos os programas possuem tratamento básico de erros.