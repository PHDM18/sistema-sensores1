#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 128
#define MAX_SENSORES 100
#define MAX_LEITURAS_POR_SENSOR 2000 

typedef struct {
    long timestamp;
    char id[20];
    char valor[32];
} Leitura;

typedef struct {
    char id[20];
    char nome_arquivo[30];
    Leitura *leituras; 
    int total_leituras;
} Sensor;


int comparar_leituras_decrescente(const void *a, const void *b) {
    Leitura *leituraA = (Leitura *)a;
    Leitura *leituraB = (Leitura *)b;
    if (leituraA->timestamp < leituraB->timestamp) return 1; 
    if (leituraA->timestamp > leituraB->timestamp) return -1; 
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <arquivo_entrada>\n", argv[0]);
        return 1;
    }

    FILE *entrada = fopen(argv[1], "r");
    if (!entrada) {
        perror("Erro ao abrir arquivo de entrada");
        return 1;
    }

    Sensor sensores[MAX_SENSORES];
    int total_sensores = 0;
    char linha[MAX_LINE];

   
    for (int i = 0; i < MAX_SENSORES; i++) {
        sensores[i].leituras = NULL; 
        sensores[i].total_leituras = 0;
    }

    while (fgets(linha, sizeof(linha), entrada)) {
       
        char temp_linha[MAX_LINE];
        strcpy(temp_linha, linha);

        char *timestamp_str = strtok(temp_linha, " ");
        char *id = strtok(NULL, " ");
        char *valor = strtok(NULL, "\n");

        if (!timestamp_str || !id || !valor) {
            fprintf(stderr, "Formato de linha inválido: %s", linha); 
            continue;
        }

        long timestamp = atol(timestamp_str);

        int encontrado_idx = -1;
        for (int i = 0; i < total_sensores; i++) {
            if (strcmp(sensores[i].id, id) == 0) {
                encontrado_idx = i;
                break;
            }
        }

        if (encontrado_idx == -1) { 
            if (total_sensores < MAX_SENSORES) {
                encontrado_idx = total_sensores;
                strcpy(sensores[encontrado_idx].id, id);
                sprintf(sensores[encontrado_idx].nome_arquivo, "%s.txt", id);
                
                sensores[encontrado_idx].leituras = (Leitura *)malloc(MAX_LEITURAS_POR_SENSOR * sizeof(Leitura));
                if (!sensores[encontrado_idx].leituras) {
                    perror("Erro de alocação de memória para leituras do novo sensor");
                   
                    for (int j = 0; j < total_sensores; j++) {
                        if (sensores[j].leituras != NULL) {
                            free(sensores[j].leituras);
                        }
                    }
                    fclose(entrada);
                    return 1;
                }
                sensores[encontrado_idx].total_leituras = 0; 
                total_sensores++;
            } else {
                fprintf(stderr, "Erro: Limite de sensores excedido. Apenas os primeiros %d sensores serão processados. Linha ignorada: %s", MAX_SENSORES, linha);
                continue; 
            }
        }

        // Adiciona a leitura ao sensor encontrado ou recém-criado
        if (sensores[encontrado_idx].total_leituras < MAX_LEITURAS_POR_SENSOR) {
            sensores[encontrado_idx].leituras[sensores[encontrado_idx].total_leituras].timestamp = timestamp;
            strcpy(sensores[encontrado_idx].leituras[sensores[encontrado_idx].total_leituras].id, id);
            strcpy(sensores[encontrado_idx].leituras[sensores[encontrado_idx].total_leituras].valor, valor);
            sensores[encontrado_idx].total_leituras++;
        } else {
            fprintf(stderr, "Aviso: Limite de leituras por sensor excedido para '%s'. Algumas leituras podem ser ignoradas. Linha: %s", id, linha);
        }
    }

    fclose(entrada);

    // Ordena e escreve os dados para cada sensor
    for (int i = 0; i < total_sensores; i++) {
        if (sensores[i].leituras == NULL) { // Garante que a memória foi alocada
            continue;
        }
        // Ordena as leituras do sensor atual em ordem decrescente de timestamp
        qsort(sensores[i].leituras, sensores[i].total_leituras, sizeof(Leitura), comparar_leituras_decrescente);

        FILE *saida_sensor = fopen(sensores[i].nome_arquivo, "w");
        if (!saida_sensor) {
            perror("Erro ao criar arquivo do sensor");
            free(sensores[i].leituras); // Libera memória mesmo em caso de erro na abertura do arquivo
            continue;
        }

        for (int j = 0; j < sensores[i].total_leituras; j++) {
            fprintf(saida_sensor, "%ld %s %s\n", sensores[i].leituras[j].timestamp, sensores[i].leituras[j].id, sensores[i].leituras[j].valor);
        }
        fclose(saida_sensor);
        free(sensores[i].leituras); // Libera a memória após escrever no arquivo
    }

    printf("Organização concluída. Arquivos gerados por sensor em ordem decrescente de timestamp.\n");
    return 0;
}