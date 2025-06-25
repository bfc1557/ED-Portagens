#ifndef EXPORT_H
#define EXPORT_H

#include "../libs/bdados.h"
#include "../structs/distancia/distancia.h"
#include "../structs/dono/dono.h"
#include "../structs/passagem/passagem.h"
#include "../structs/sensor/sensor.h"
#include "../structs/veiculo/veiculo.h"

// Declaração da função exportToXML com parâmetros na ordem correta
void exportToXML(DonoHashTable *donoTable, VeiculoHashTable *veiculoTable,
                SensorList sensorList, PassagemList passagemList,
                DistanciaList distanciaList, const char *filename);

#endif