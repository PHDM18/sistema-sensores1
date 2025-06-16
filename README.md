# sistema-sensores1

# Sistema de Sensores Industriais

Este projeto contém três programas C para gerenciar leituras de sensores industriais.

## programa1.c – Organização dos Dados

Lê um arquivo de entrada de leituras e organiza-as em arquivos separados por sensor. As leituras em cada arquivo de sensor são ordenadas por timestamp em ordem decrescente.

**Uso:** `./organizar <arquivo_entrada>`

## programa2.c – Consulta por Instante

Pesquisa a leitura de um sensor específico mais próxima de um timestamp informado. A busca utiliza algoritmo de busca binária em arquivos ordenados em ordem decrescente.

**Uso:** `./buscar <id_sensor> <timestamp_consultado>`

## programa3.c – Geração de Arquivo de Teste

Gera um arquivo de texto com leituras aleatórias de sensores para testes.

**Argumentos:**
-   `<dia_inicio> <mes_inicio> <ano_inicio>`: Data de início do intervalo.
-   `<dia_fim> <mes_fim> <ano_fim>`: Data de fim do intervalo.
-   `<sensor_nome> <tipo_dado>`: Pares de nome do sensor e tipo de dado (pode ser repetido para múltiplos sensores).

**Tipos de dados suportados:**
-   `CONJ_Z`: Inteiro
-   `CONJ_Q`: Float
-   `TEXTO`: String 
-   `BINARIO`: Booleano 

Para cada sensor, são geradas 2000 leituras com timestamps aleatórios dentro do intervalo fornecido.

**Uso:** `./gerar <dia_inicio> <mes_inicio> <ano_inicio> <dia_fim> <mes_fim> <ano_fim> [<sensor_1_nome> <sensor_1_tipo>] [<sensor_2_nome> <sensor_2_tipo>] ...`

**Exemplo:** `./gerar 1 1 2023 31 12 2023 temp CONJ_Z pressao CONJ_Q`

## Como compilar o codigo

Use `gcc`:

```bash
gcc programa1.c -o organizar
gcc programa2.c -o buscar -lm
gcc programa3.c -o gerar
