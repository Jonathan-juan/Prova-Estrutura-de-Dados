#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

typedef struct {
    long timestamp; 
    char id_sensor[17]; 
    char valor[256]; 
} Leitura;

time_t converter_para_timestamp(int dia, int mes, int ano, int hora, int min, int seg) {
    struct tm t;
    t.tm_year = ano - 1900;
    t.tm_mon = mes - 1;
    t.tm_mday = dia;
    t.tm_hour = hora;
    t.tm_min = min;
    t.tm_sec = seg;
    t.tm_isdst = -1;

    time_t timestamp = mktime(&t);
    return (timestamp == -1) ? -1 : timestamp;
}

int busca_binaria_mais_proxima(Leitura *leituras, int num_leituras, long timestamp_alvo) {
    int baixo = 0;
    int alto = num_leituras - 1;
    int meio;
    int indice_mais_proximo = 0;
    long menor_diferenca = -1;

    while (baixo <= alto) {
        meio = baixo + (alto - baixo) / 2;
        long diferenca = labs(leituras[meio].timestamp - timestamp_alvo);

        if (menor_diferenca == -1 || diferenca < menor_diferenca) {
            menor_diferenca = diferenca;
            indice_mais_proximo = meio;
        }

        if (leituras[meio].timestamp == timestamp_alvo) {
            return meio;
        } else if (leituras[meio].timestamp < timestamp_alvo) {
            baixo = meio + 1;
        } else {
            alto = meio - 1;
        }
    }

    if (indice_mais_proximo > 0) {
        long dif_anterior = labs(leituras[indice_mais_proximo - 1].timestamp - timestamp_alvo);
        if (dif_anterior < menor_diferenca) {
            menor_diferenca = dif_anterior;
            indice_mais_proximo--;
        }
    }

    if (indice_mais_proximo < num_leituras - 1) {
        long dif_proximo = labs(leituras[indice_mais_proximo + 1].timestamp - timestamp_alvo);
        if (dif_proximo < menor_diferenca) {
            indice_mais_proximo++;
        }
    }

    return indice_mais_proximo;
}

int main(int argc, char *argv[]) {
    if (argc != 8) {
        printf("Uso: %s <id_sensor> <dd> <mm> <aaaa> <hh> <min> <ss>\n", argv[0]);
        return 1;
    }

    char *id_sensor_alvo = argv[1];
    int dd = atoi(argv[2]);
    int mm = atoi(argv[3]);
    int aaaa = atoi(argv[4]);
    int hh = atoi(argv[5]);
    int min = atoi(argv[6]);
    int ss = atoi(argv[7]);

    if (dd < 1 || dd > 31 || mm < 1 || mm > 12 || aaaa < 1970 || hh < 0 || hh > 23 || min < 0 || min > 59 || ss < 0 || ss > 59) {
        printf("Erro: Data e/ou hora inválidas. Verifique os valores.\n");
        return 1;
    }

    time_t timestamp_alvo = converter_para_timestamp(dd, mm, aaaa, hh, min, ss);
    if (timestamp_alvo == -1) {
        printf("Erro: Data e hora fornecidas resultam em um timestamp inválido.\n");
        return 1;
    }

    char nome_arquivo_sensor[30];
    sprintf(nome_arquivo_sensor, "%s.txt", id_sensor_alvo);

    FILE *arquivo_sensor = fopen(nome_arquivo_sensor, "r");
    if (arquivo_sensor == NULL) {
        printf("Erro: Arquivo do sensor '%s' não encontrado. Certifique-se de que o Programa 1 foi executado.\n", id_sensor_alvo);
        return 1;
    }

    Leitura *leituras = NULL;
    int num_leituras = 0;
    int capacidade_leituras = 10;

    leituras = (Leitura *) malloc(capacidade_leituras * sizeof(Leitura));
    if (leituras == NULL) {
        printf("Erro de alocação de memória.\n");
        fclose(arquivo_sensor);
        return 1;
    }

    char linha[300];
    long temp_timestamp;
    char temp_id_sensor[17];
    char *ptr_valor;

    while (fgets(linha, sizeof(linha), arquivo_sensor) != NULL) {
        if (sscanf(linha, "%ld %16s", &temp_timestamp, temp_id_sensor) == 2) {
            ptr_valor = strchr(linha, ' ');
            if (ptr_valor != NULL) {
                ptr_valor = strchr(ptr_valor + 1, ' ');
            }

            if (ptr_valor != NULL) {
                ptr_valor++;
                size_t len = strlen(ptr_valor);
                if (len > 0 && ptr_valor[len - 1] == '\n') {
                    ptr_valor[len - 1] = '\0';
                }

                if (num_leituras >= capacidade_leituras) {
                    capacidade_leituras *= 2;
                    leituras = (Leitura *) realloc(leituras, capacidade_leituras * sizeof(Leitura));
                    if (leituras == NULL) {
                        printf("Erro de realocação de memória.\n");
                        fclose(arquivo_sensor);
                        return 1;
                    }
                }

                leituras[num_leituras].timestamp = temp_timestamp;
                strcpy(leituras[num_leituras].id_sensor, temp_id_sensor);
                strcpy(leituras[num_leituras].valor, ptr_valor);
                num_leituras++;
            } else {
                printf("Aviso: Linha ignorada (valor ausente): %s", linha);
            }
        } else {
            printf("Aviso: Linha ignorada (timestamp ou ID ausente): %s", linha);
        }
    }
    fclose(arquivo_sensor);

    if (num_leituras == 0) {
        printf("Nenhuma leitura encontrada para o sensor '%s'.\n", id_sensor_alvo);
        free(leituras);
        return 1;
    }

    int indice_encontrado = busca_binaria_mais_proxima(leituras, num_leituras, timestamp_alvo);

    printf("Leitura mais próxima encontrada:\n");
    printf("  Sensor ID: %s\n", leituras[indice_encontrado].id_sensor);

    time_t ts_encontrado = leituras[indice_encontrado].timestamp;
    struct tm *info_tempo = localtime(&ts_encontrado);
    char buffer_data_hora[80];
    strftime(buffer_data_hora, sizeof(buffer_data_hora), "%Y-%m-%d %H:%M:%S", info_tempo);
    printf("  Data e Hora: %s (Timestamp: %ld)\n", buffer_data_hora, ts_encontrado);

    printf("  Valor: %s\n", leituras[indice_encontrado].valor);

    free(leituras);
    return 0;
}