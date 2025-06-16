#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

time_t converter_para_timestamp(int dia, int mes, int ano, int hora, int min, int seg) {
    struct tm t = {0};

    t.tm_year = ano - 1900;
    t.tm_mon = mes - 1;
    t.tm_mday = dia;
    t.tm_hour = hora;
    t.tm_min = min;
    t.tm_sec = seg;
    t.tm_isdst = -1;

    time_t timestamp = mktime(&t);
    if (timestamp == (time_t)-1) {
        fprintf(stderr, "Erro: Data/hora inválida fornecida.\n");
    }
    return timestamp;
}

time_t gerar_timestamp_aleatorio(time_t inicio, time_t fim) {
    if (inicio > fim) {
        time_t temp = inicio;
        inicio = fim;
        fim = temp;
    }
    return inicio + rand() % (fim - inicio + 1);
}

int main(int argc, char *argv[]) {
    srand(time(NULL));

    if (argc < 7 || (argc - 7) % 2 != 0) {
        printf("Uso: %s <dia_inicio> <mes_inicio> <ano_inicio> <dia_fim> <mes_fim> <ano_fim> [<sensor_nome> <tipo_dado>] ...\n", argv[0]);
        printf("Tipos de dados suportados: CONJ_Z, CONJ_Q, TEXTO, BINARIO\n");
        return 1;
    }

    int dia_inicio = atoi(argv[1]);
    int mes_inicio = atoi(argv[2]);
    int ano_inicio = atoi(argv[3]);
    int dia_fim = atoi(argv[4]);
    int mes_fim = atoi(argv[5]);
    int ano_fim = atoi(argv[6]);

    time_t t_inicio = converter_para_timestamp(dia_inicio, mes_inicio, ano_inicio, 0, 0, 0);
    time_t t_fim = converter_para_timestamp(dia_fim, mes_fim, ano_fim, 23, 59, 59);

    if (t_inicio == (time_t)-1 || t_fim == (time_t)-1) {
        return 1;
    }
    if (t_inicio > t_fim) {
        fprintf(stderr, "Erro: Data de início não pode ser maior que a data de fim.\n");
        return 1;
    }

    FILE *arquivo = fopen("leituras.txt", "w");
    if (!arquivo) {
        perror("Erro ao abrir arquivo de saída");
        return 1;
    }

    for (int i = 7; i < argc; i += 2) {
        char *sensor_nome = argv[i];
        char *tipo_dado = argv[i + 1];

        for (int j = 0; j < 2000; j++) {
            long ts = gerar_timestamp_aleatorio(t_inicio, t_fim);
            
            if (strcmp(tipo_dado, "CONJ_Z") == 0) {
                fprintf(arquivo, "%ld %s %d\n", ts, sensor_nome, rand() % 1000);
            } else if (strcmp(tipo_dado, "CONJ_Q") == 0) {
                fprintf(arquivo, "%ld %s %.2f\n", ts, sensor_nome, (float)(rand() % 10000) / 100.0);
            } else if (strcmp(tipo_dado, "BINARIO") == 0) {
                fprintf(arquivo, "%ld %s %s\n", ts, sensor_nome, rand() % 2 ? "true" : "false");
            } else if (strcmp(tipo_dado, "TEXTO") == 0) {
                fprintf(arquivo, "%ld %s codigo%04d\n", ts, sensor_nome, rand() % 10000);
            } else {
                fprintf(stderr, "Tipo de dado desconhecido para o sensor %s: %s. Ignorando este sensor.\n", sensor_nome, tipo_dado);
                break;
            }
        }
    }

    fclose(arquivo);
    printf("Arquivo de teste 'leituras.txt' gerado com sucesso!\n");
    return 0;
}