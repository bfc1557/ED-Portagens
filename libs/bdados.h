#ifndef BDADOS_H
#define BDADOS_H

#include "../structs/dono/dono.h"
#include "../structs/veiculo/veiculo.h"
#include "../structs/sensor/sensor.h"
#include "../structs/passagem/passagem.h"
#include "../structs/distancia/distancia.h"

typedef struct {
    DonoHashTable *donos;
    VeiculoHashTable *veiculos;
    SensorList sensores;
    PassagemList passagens;
    DistanciaList distancias;
    double total_memoria;
} BDados;

BDados* InicializaPrograma(const char* nome);
void FinalizarPrograma(BDados* bd);
void CarregarDados(BDados* bd, const char* dir_dados);
void CalcularMemoria(BDados* bd);

#endif