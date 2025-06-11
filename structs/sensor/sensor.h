#ifndef SENSOR_H
#define SENSOR_H

typedef struct {
    int codSensor;
    char designacao[100];
    char latitude[50];
    char longitude[50];
} Sensor;

typedef struct SensorNode {
    Sensor sensor;
    struct SensorNode *next;
} SensorNode;

typedef SensorNode* SensorList;

SensorList criar_lista_sensores();
void liberar_lista_sensores(SensorList list);
void inserir_sensor(SensorList* list, Sensor sensor);
Sensor* buscar_sensor(SensorList list, int codSensor);
void carregar_sensores(SensorList* list, const char* filename);
int calcular_memoria_sensores(SensorList list);

#endif