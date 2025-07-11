/**
 * @file sensor.c
 * @brief Implementação das operações para gestão de sensores
 * @details Contém as funções para criar, manipular e consultar uma lista de sensores
 */

#include "sensor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Cria uma lista vazia de sensores
 * @return Lista vazia (NULL)
 */
SensorList criar_lista_sensores() {
    return NULL;
}

/**
 * @brief Liberta toda a memória ocupada pela lista de sensores
 * @param list Lista de sensores a libertar
 */
void liberar_lista_sensores(SensorList list) {
    SensorNode* current = list;
    while (current) {
        SensorNode* temp = current;
        current = current->next;
        free(temp);
    }
}

/**
 * @brief Insere um novo sensor no início da lista
 * @param list Ponteiro para a lista de sensores
 * @param sensor Dados do sensor a inserir
 */
void inserir_sensor(SensorList* list, Sensor sensor) {
    SensorNode* newNode = (SensorNode*)malloc(sizeof(SensorNode));
    if (!newNode) {
        perror("Erro ao alocar memória para novo sensor");
        exit(EXIT_FAILURE);
    }
    newNode->sensor = sensor;
    newNode->next = *list;
    *list = newNode;
}

/**
 * @brief Procura um sensor pelo seu código
 * @param list Lista de sensores
 * @param codSensor Código do sensor a procurar
 * @return Ponteiro para o sensor encontrado ou NULL se não existir
 */
Sensor* buscar_sensor(SensorList list, int codSensor) {
    SensorNode* current = list;
    while (current) {
        if (current->sensor.codSensor == codSensor) {
            return &current->sensor;
        }
        current = current->next;
    }
    return NULL;
}

/**
 * @brief Carrega sensores a partir de um ficheiro de texto
 * @param list Ponteiro para a lista de sensores
 * @param filename Nome do ficheiro a carregar
 * @note Formato esperado do ficheiro:
 *       codSensor\tdesignação\tlatitude\tlongitude\n
 */
void carregar_sensores(SensorList* list, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir ficheiro de sensores");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        Sensor sensor;
        if (sscanf(line, "%d\t%99[^\t]\t%49[^\t]\t%49[^\t]",
                  &sensor.codSensor, sensor.designacao,
                  sensor.latitude, sensor.longitude) == 4) {
            inserir_sensor(list, sensor);
        }
    }
    fclose(file);
}

/**
 * @brief Calcula a memória total ocupada pela lista de sensores
 * @param list Lista de sensores
 * @return Total de bytes utilizados
 * @note Inclui tamanho dos nós e strings alocadas
 */
int calcular_memoria_sensores(SensorList list) {
    int total = 0;
    SensorNode* current = list;
    while (current) {
        total += sizeof(SensorNode) + strlen(current->sensor.designacao) +
                 strlen(current->sensor.latitude) + strlen(current->sensor.longitude);
        current = current->next;
    }
    return total;
}

/**
 * @brief Exporta a lista de sensores para formato CSV
 * @param list Lista de sensores
 * @param filename Nome do ficheiro CSV de destino
 * @note Formato do CSV:
 *       codSensor,designacao,latitude,longitude
 */
void exportSensorToCSV(SensorList list, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Erro ao abrir ficheiro CSV");
        return;
    }

    // Cabeçalho do CSV
    fprintf(file, "codSensor,designacao,latitude,longitude\n");
    
    SensorNode *current = list;
    while (current != NULL) {
        fprintf(file, "%d,\"%s\",\"%s\",\"%s\"\n", 
                current->sensor.codSensor,
                current->sensor.designacao,
                current->sensor.latitude,
                current->sensor.longitude);
        current = current->next;
    }
    
    fclose(file);
}

/**
 * @brief Guarda a lista de sensores num ficheiro de texto
 * @param list Lista de sensores
 * @param filename Nome do ficheiro de destino
 * @note Formato do ficheiro:
 *       codSensor\tdesignação\tlatitude\tlongitude\n
 */
void salvar_sensores(SensorList list, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Erro ao abrir ficheiro de sensores");
        return;
    }

    SensorNode* current = list;
    while (current != NULL) {
        fprintf(file, "%d\t%s\t%s\t%s\n",
                current->sensor.codSensor,
                current->sensor.designacao,
                current->sensor.latitude,
                current->sensor.longitude);
        current = current->next;
    }

    fclose(file);
}