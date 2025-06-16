#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
    long timestamp;
    char id[20];
    char valor[32];
} Leitura;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s <id_sensor> <timestamp_consultado>\n", argv[0]);
        return 1;
    }

    char nome_arquivo[32];
    sprintf(nome_arquivo, "%s.txt", argv[1]);

    long alvo = atol(argv[2]);

    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir arquivo do sensor");
        return 1;
    }

    Leitura *leituras = (Leitura *)malloc(2000 * sizeof(Leitura));
    if (!leituras) {
        perror("Erro de alocação de memória");
        fclose(arquivo);
        return 1;
    }

    int total = 0;
    while (fscanf(arquivo, "%ld %s %s", &leituras[total].timestamp, leituras[total].id, leituras[total].valor) == 3) {
        total++;
        if (total >= 2000) {
            fprintf(stderr, "Aviso: Limite de leituras por sensor excedido (%d). Apenas as primeiras leituras foram lidas.\n", 2000);
            break;
        }
    }

    fclose(arquivo);

    if (total == 0) {
        printf("Nenhuma leitura encontrada para o sensor '%s'.\n", argv[1]);
        free(leituras);
        return 0;
    }

    int esq = 0, dir = total - 1;
    int melhor_idx = -1;

    while (esq <= dir) {
        int meio = esq + (dir - esq) / 2;
        
        if (leituras[meio].timestamp == alvo) {
            melhor_idx = meio;
            break;
        } else if (leituras[meio].timestamp > alvo) {
            melhor_idx = meio;
            esq = meio + 1;
        } else {
            melhor_idx = meio;
            dir = meio - 1;
        }
    }

    if (melhor_idx != -1) {
        if (melhor_idx - 1 >= 0 && labs(leituras[melhor_idx - 1].timestamp - alvo) < labs(leituras[melhor_idx].timestamp - alvo)) {
            melhor_idx = melhor_idx - 1;
        }
        if (melhor_idx + 1 < total && labs(leituras[melhor_idx + 1].timestamp - alvo) < labs(leituras[melhor_idx].timestamp - alvo)) {
            melhor_idx = melhor_idx + 1;
        }
    } else if (total > 0) {
        long diff_primeiro = labs(leituras[0].timestamp - alvo);
        long diff_ultimo = labs(leituras[total - 1].timestamp - alvo);

        if (diff_primeiro <= diff_ultimo) {
            melhor_idx = 0;
        } else {
            melhor_idx = total - 1;
        }
    }

    if (melhor_idx != -1) {
        printf("Leitura mais proxima: %ld %s %s\n", leituras[melhor_idx].timestamp, leituras[melhor_idx].id, leituras[melhor_idx].valor);
    } else {
        printf("Nenhuma leitura proxima encontrada.\n");
    }

    free(leituras);
    return 0;
}