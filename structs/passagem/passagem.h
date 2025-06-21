#ifndef PASSAGEM_H
#define PASSAGEM_H

#include <time.h>
#include "../veiculo/veiculo.h"
#include "../../libs/utils.h"

typedef struct {
    int idSensor;
    int codVeiculo;
    time_t data;
    int tipoRegisto; // 0=entrada, 1=saída
} Passagem;

typedef struct PassagemNode {
    Passagem passagem;
    struct PassagemNode *next;
} PassagemNode;

typedef PassagemNode* PassagemList;

PassagemList criar_lista_passagens();
void liberar_lista_passagens(PassagemList list);
void inserir_passagem(PassagemList* list, Passagem passagem);
void carregar_passagens(PassagemList* list, const char* filename, int batch_size);
int calcular_memoria_passagens(PassagemList list);
void listar_passagens_periodo(PassagemList list, time_t inicio, time_t fim);
void listar_veiculos_periodo(PassagemList passagens, VeiculoHashTable* veiculos, time_t inicio, time_t fim);
void registar_passagem(PassagemList* list, VeiculoHashTable* veiculos);
void salvar_passagens(PassagemList list, const char* filename);

#endif