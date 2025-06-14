# Prova-Estrutura-de-Dados
Este repositório tem três programas em C feitos para a prova de Estrutura de Dados. Eles simulam um sistema que organiza e consulta dados de sensores.

## Estrutura do Projeto

O projeto tem os seguintes arquivos:

`p1_organizacao.c`: Programa que organiza os dados.
`p2_consulta.c`: Programa que faz consultas por data/hora.
`p3_geracao.c`: Programa que gera dados de teste.
`README.md`: Explica o projeto e como usar os programas.

## Como Compilar e Executar

Para compilar os programas, use um compilador C (como o GCC). No terminal, entre na pasta trabalho_final e digite:

```bash
gcc -o organizacao organizacao.c
gcc -o consulta consulta.c -lm
gcc -o geracao geracao.c
```

## Programa 1: Organização dos Dados (`p1_organizacao.c`)

Este programa lê um arquivo com dados de vários sensores, separa cada sensor em um arquivo diferente e organiza as leituras por ordem de tempo.

### Formato do Arquivo de Entrada

Cada linha do arquivo deve seguir este formato:

<TIMESTAMP> <ID_SENSOR> <VALOR>

<TIMESTAMP>: Momento da leitura, no formato Unix (número inteiro).

<ID_SENSOR>: Nome do sensor (sem espaços, até 16 caracteres).

<VALOR>: Leitura do sensor — pode ser inteiro, booleano (true ou false), número decimal ou uma palavra de até 16 letras.

### Uso

```bash
./organizacao <nome_do_arquivo_entrada>
```

**Exemplo:**

```bash
./organizacao dados_brutos.txt
```

O programa vai criar arquivos como SENSOR1.txt, SENSOR2.txt com os dados separados e organizados por sensor.

## Programa 2: Consulta por Instante (`p2_consulta.c`)

Este programa procura a leitura de um sensor que esteja mais próxima da data e hora informadas, usando busca binária para ser mais rápido.

### Uso

```bash
./consulta <id_sensor> <dd> <mm> <aaaa> <hh> <min> <ss>
```

<id_sensor>: Nome do sensor (ex: TEMP, UMID)

<dd> <mm> <aaaa> <hh> <min> <ss>: Data e hora da consulta

**Exemplo:**

```bash
./consulta TEMP 15 06 2025 10 30 00
```

O programa mostra a leitura mais próxima dessa data e hora para o sensor escolhido.

## Programa 3: Geração de Arquivo de Teste (`p3_geracao.c`)

Este programa cria um arquivo com leituras aleatórias de vários sensores, dentro de um intervalo de tempo escolhido e com tipos de dados diferentes.

### Uso

```bash
./geracao <dd_inicio> <mm_inicio> <aaaa_inicio> <hh_inicio> <min_inicio> <ss_inicio> <dd_fim> <mm_fim> <aaaa_fim> <hh_fim> <min_fim> <ss_fim> <sensor1_id> <sensor1_tipo> [sensor2_id sensor2_tipo ...]
```

<dd_inicio> ... <ss_inicio>: Data e hora de começo.

<dd_fim> ... <ss_fim>: Data e hora de fim.

<sensorX_id>: Nome do sensor (ex: TEMP, PRES).

<sensorX_tipo>: Tipo do dado do sensor. Pode ser: inteiro, booleano, racional ou string.

Para cada sensor informado, o programa cria 2000 leituras aleatórias.

**Exemplo:**

```bash
./geracao 01 01 2024 00 00 00 31 12 2024 23 59 59 TEMP inteiro UMID racional LUZ booleano MSG string
```
Isso vai gerar um arquivo chamado dados_teste.txt com os dados

## Considerações sobre a Implementação

Código simples e claro, usando conceitos básicos de C como structs, ponteiros, alocação dinâmica, arquivos e strings.

O Programa 2 usa busca binária para achar a leitura mais próxima, deixando a busca rápida mesmo com muitos dados.

O valor da leitura é guardado como string, facilitando o uso de diferentes tipos (inteiro, booleano, racional, texto) sem complicar o código.

O programa faz checagens básicas de erros na entrada e na abertura dos arquivos.

O código foi feito do zero, com foco em simplicidade e sem uso de soluções
