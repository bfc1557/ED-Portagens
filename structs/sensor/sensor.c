#include "sensor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

SensorList criar_lista_sensores() {
    return NULL;
}

void liberar_lista_sensores(SensorList list) {
    SensorNode* current = list;
    while (current) {
        SensorNode* temp = current;
        current = current->next;
        free(temp);
    }
}

void inserir_sensor(SensorList* list, Sensor sensor) {
    SensorNode* newNode = (SensorNode*)malloc(sizeof(SensorNode));
    newNode->sensor = sensor;
    newNode->next = *list;
    *list = newNode;
}

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

void carregar_sensores(SensorList* list, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir arquivo de sensores");
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