#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

time_t converter_para_timestamp(int dia, int mes, int ano, int hora, int min, int seg){
    struct tm t;
    t.tm_year = ano - 1900;
    t.tm_mon = mes - 1;
    t.tm_mday = dia;
    t.tm_hour = hora;
    t.tm_min = min;
    t.tm_sec = seg;
    t.tm_isdst = -1; 
    time_t timestamp = mktime(&t);
    if (timestamp == -1){
        return -1; 
    }
    return timestamp;
}

time_t gerar_timestamp_aleatorio(time_t inicio, time_t fim){
    if (inicio == -1 || fim == -1 || inicio > fim){
        return -1; 
    }
    return inicio + rand() % (fim - inicio + 1);
}

void gerar_valor_aleatorio_por_tipo(char *buffer, const char *tipo_dado){
    if (strcmp(tipo_dado, "inteiro") == 0){
        sprintf(buffer, "%d", rand() % 1000); 
    } else if (strcmp(tipo_dado, "booleano") == 0){
        sprintf(buffer, "%s", (rand() % 2 == 0) ? "true" : "false"); 
    } else if (strcmp(tipo_dado, "racional") == 0) {
        sprintf(buffer, "%.2f", (float)rand() / RAND_MAX * 100.0); 
    } else if (strcmp(tipo_dado, "string") == 0){
        
        int len = 1 + rand() % 16; 
        for (int i = 0; i < len; i++){
            buffer[i] = 'A' + (rand() % 26); 
        }
        buffer[len] = '\0';
    } else{
        strcpy(buffer, "UNKNOWN");
    }
}

int main(int argc, char *argv[]){
  
    if (argc < 13 || (argc - 13) % 2 != 0){
        printf("Uso: %s <dd_inicio> <mm_inicio> <aaaa_inicio> <hh_inicio> <min_inicio> <ss_inicio> <dd_fim> <mm_fim> <aaaa_fim> <hh_fim> <min_fim> <ss_fim> <sensor1_id> <sensor1_tipo> [sensor2_id sensor2_tipo ... ]\n", argv[0]);
        return 1;
    }

    int idx = 1;
    int dd_inicio = atoi(argv[idx++]);
    int mm_inicio = atoi(argv[idx++]);
    int aaaa_inicio = atoi(argv[idx++]);
    int hh_inicio = atoi(argv[idx++]);
    int min_inicio = atoi(argv[idx++]);
    int ss_inicio = atoi(argv[idx++]);

    int dd_fim = atoi(argv[idx++]);
    int mm_fim = atoi(argv[idx++]);
    int aaaa_fim = atoi(argv[idx++]);
    int hh_fim = atoi(argv[idx++]);
    int min_fim = atoi(argv[idx++]);
    int ss_fim = atoi(argv[idx++]);

    if (dd_inicio < 1 || dd_inicio > 31 || mm_inicio < 1 || mm_inicio > 12 || aaaa_inicio < 1970 || hh_inicio < 0 || hh_inicio > 23 || min_inicio < 0 || min_inicio > 59 || ss_inicio < 0 || ss_inicio > 59) {
        printf("Erro: Data e/ou hora de início inválidas. Verifique os valores.\n");
        return 1;
    }

    if (dd_fim < 1 || dd_fim > 31 || mm_fim < 1 || mm_fim > 12 || aaaa_fim < 1970 || hh_fim < 0 || hh_fim > 23 || min_fim < 0 || min_fim > 59 || ss_fim < 0 || ss_fim > 59) {
        printf("Erro: Data e/ou hora de fim inválidas. Verifique os valores.\n");
        return 1;
    }

    time_t timestamp_inicio = converter_para_timestamp(dd_inicio, mm_inicio, aaaa_inicio, hh_inicio, min_inicio, ss_inicio);
    time_t timestamp_fim = converter_para_timestamp(dd_fim, mm_fim, aaaa_fim, hh_fim, min_fim, ss_fim);

    if (timestamp_inicio == -1 || timestamp_fim == -1) {
        printf("Erro: Data e hora fornecidas resultam em um timestamp inválido (ex: data inexistente).\n");
        return 1;
    }
    if (timestamp_inicio > timestamp_fim) {
        printf("Erro: A data de início é posterior à data de fim.\n");
        return 1;
    }

    srand(time(NULL)); 

    FILE *arquivo_saida = fopen("dados_teste.txt", "w");
    if (arquivo_saida == NULL){
        printf("Erro ao criar o arquivo de saída.\n");
        return 1;
    }

    for (int i = idx; i < argc; i += 2) {
        char *id_sensor = argv[i];
        char *tipo_dado = argv[i+1];

        printf("Gerando 2000 leituras para o sensor %s (tipo: %s)...\n", id_sensor, tipo_dado);

        for (int j = 0; j < 2000; j++) {
            time_t ts = gerar_timestamp_aleatorio(timestamp_inicio, timestamp_fim);
            char valor_str[256];
            gerar_valor_aleatorio_por_tipo(valor_str, tipo_dado);
            fprintf(arquivo_saida, "%ld %s %s\n", ts, id_sensor, valor_str);
        }
    }

    fclose(arquivo_saida);
    printf("Arquivo 'dados_teste.txt' gerado com sucesso.\n");

    return 0;
}


