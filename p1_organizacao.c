#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct{
    long timestamp; 
    char id_sensor[17]; 
    char valor[256]; 
} Leitura;

int comparar_leituras_custom(Leitura *leitura1, Leitura *leitura2){
    if (leitura1->timestamp < leitura2->timestamp) return -1;
    if (leitura1->timestamp > leitura2->timestamp) return 1;
    return 0;
}

void bubble_sort(Leitura *arr, int n){
    int i, j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (comparar_leituras_custom(&arr[j], &arr[j+1]) > 0){
                
                Leitura temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp
            }
        }
    }
}

int main(int argc, char *argv[]){
    
    if (argc != 2) {
        printf("Uso: %s <nome_do_arquivo_entrada>\n", argv[0]);
        return 1;
    }

    FILE *arquivo_entrada = fopen(argv[1], "r");
    if (arquivo_entrada == NULL) {
        printf("Erro ao abrir o arquivo de entrada: %s\n", argv[1]);
        return 1;
    }

    Leitura *todas_leituras = NULL;
    int num_leituras = 0;
    int capacidade_leituras = 10; 

    todas_leituras = (Leitura *) malloc(capacidade_leituras * sizeof(Leitura));
    if (todas_leituras == NULL) {
        printf("Erro de alocação de memória.\n");
        fclose(arquivo_entrada);
        return 1;
    }

    char linha[300]; 
    long temp_timestamp;
    char temp_id_sensor[17];
    char *ptr_valor;

    while (fgets(linha, sizeof(linha), arquivo_entrada) != NULL){
        
        if (sscanf(linha, "%ld %16s", &temp_timestamp, temp_id_sensor) == 2){
            
            ptr_valor = strchr(linha, ' ')
            if (ptr_valor != NULL) {
                ptr_valor = strchr(ptr_valor + 1, ' '); 
            }

            if (ptr_valor != NULL) {
                ptr_valor++; 
                size_t len = strlen(ptr_valor);
                if (len > 0 && ptr_valor[len - 1] == '\n'){
                    ptr_valor[len - 1] = '\0';
                }

                if (num_leituras >= capacidade_leituras){
                    capacidade_leituras *= 2;
                    todas_leituras = (Leitura *) realloc(todas_leituras, capacidade_leituras * sizeof(Leitura));
                    if (todas_leituras == NULL) {
                        printf("Erro de realocação de memória.\n");
                        fclose(arquivo_entrada);
                        return 1;
                    }
                }
                todas_leituras[num_leituras].timestamp = temp_timestamp;
                strcpy(todas_leituras[num_leituras].id_sensor, temp_id_sensor);
                strcpy(todas_leituras[num_leituras].valor, ptr_valor);
                num_leituras++;
            } else {
                printf("Aviso: Linha ignorada devido a formato inválido (valor ausente): %s", linha);
            }
        } else {
            printf("Aviso: Linha ignorada devido a formato inválido (timestamp ou ID_SENSOR ausente): %s", linha);
        }
    }
    fclose(arquivo_entrada);

    char sensores_unicos[100][17];
    int num_sensores_unicos = 0;

    for (int i = 0; i < num_leituras; i++){
        int encontrado = 0;
        for (int j = 0; j < num_sensores_unicos; j++){
            if (strcmp(todas_leituras[i].id_sensor, sensores_unicos[j]) == 0){
                encontrado = 1;
                break;
            }
        }
        if (!encontrado) {
            if (num_sensores_unicos >= 100){
                printf("Limite de sensores únicos atingido. Aumente o array 'sensores_unicos'.\n");
                break;
            }
            strcpy(sensores_unicos[num_sensores_unicos], todas_leituras[i].id_sensor);
            num_sensores_unicos++;
        }
    }

    for (int i = 0; i < num_sensores_unicos; i++){
        char nome_arquivo_saida[30];
        sprintf(nome_arquivo_saida, "%s.txt", sensores_unicos[i]);

        FILE *arquivo_saida = fopen(nome_arquivo_saida, "w");
        if (arquivo_saida == NULL){
            printf("Erro ao criar arquivo para o sensor %s.\n", sensores_unicos[i]);
            continue;
        }

        Leitura *leituras_sensor_atual = NULL;
        int num_leituras_sensor_atual = 0;
        int capacidade_sensor_atual = 10

        leituras_sensor_atual = (Leitura *) malloc(capacidade_sensor_atual * sizeof(Leitura));
        if (leituras_sensor_atual == NULL){
            printf("Erro de alocação de memória para sensor %s.\n", sensores_unicos[i]);
            fclose(arquivo_saida);
            continue;
        }

        for (int j = 0; j < num_leituras; j++){
            if (strcmp(todas_leituras[j].id_sensor, sensores_unicos[i]) == 0){
                if (num_leituras_sensor_atual >= capacidade_sensor_atual){
                    capacidade_sensor_atual *= 2;
                    leituras_sensor_atual = (Leitura *) realloc(leituras_sensor_atual, capacidade_sensor_atual * sizeof(Leitura));
                    if (leituras_sensor_atual == NULL) {
                        printf("Erro de realocação de memória para sensor %s.\n", sensores_unicos[i]);
                        break;
                    }
                }
                leituras_sensor_atual[num_leituras_sensor_atual] = todas_leituras[j];
                num_leituras_sensor_atual++;
            }
        }

        bubble_sort(leituras_sensor_atual, num_leituras_sensor_atual);

        for (int j = 0; j < num_leituras_sensor_atual; j++){
            fprintf(arquivo_saida, "%ld %s %s\n", 
                    leituras_sensor_atual[j].timestamp, 
                    leituras_sensor_atual[j].id_sensor, 
                    leituras_sensor_atual[j].valor);
        }

        fclose(arquivo_saida);
        free(leituras_sensor_atual);
        printf("Arquivo '%s' criado com %d leituras.\n", nome_arquivo_saida, num_leituras_sensor_atual);
    }

    free(todas_leituras);

    return 
}


