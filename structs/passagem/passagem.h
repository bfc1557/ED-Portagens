#ifndef PASSAGEM_H
#define PASSAGEM_H

#include <time.h>
#include "../veiculo/veiculo.h"
#include "../distancia/distancia.h"
#include "../../libs/utils.h"

typedef struct {
    int idSensor;
    int codVeiculo;
    time_t data;
    int milissegundos;    // Campo adicionado
    int tipoRegisto;      /* 0 = entrada, 1 = saída */
} Passagem;

typedef struct PassagemNode {
    Passagem passagem;
    struct PassagemNode *next;
} PassagemNode;

typedef PassagemNode* PassagemList;

/* operações básicas */
PassagemList criar_lista_passagens(void);
void liberar_lista_passagens(PassagemList list);
void inserir_passagem(PassagemList *list, Passagem passagem);

/* carregamento e estatísticas */
void carregar_passagens(PassagemList *list, const char *filename, int batch_size);
int calcular_memoria_passagens(PassagemList list);

/* listagens simples */
void listar_passagens_periodo(PassagemList list, time_t inicio, time_t fim);
void listar_veiculos_periodo(PassagemList passagens, VeiculoHashTable* veiculos, time_t inicio, time_t fim);
void listar_ranking_circulacao(PassagemList passagens, VeiculoHashTable* veiculos, DistanciaList distancias, time_t inicio, time_t fim);
void listar_ranking_marca(PassagemList passagens, VeiculoHashTable* veiculos, DistanciaList distancias, time_t inicio, time_t fim);
void listar_infracoes(PassagemList passagens, VeiculoHashTable *veiculos, DistanciaList distancias, time_t inicio, time_t fim);
void listar_ranking_infracoes(PassagemList passagens, VeiculoHashTable *veiculos, DistanciaList distancias, time_t inicio, time_t fim);
void listar_marca_maior_velocidade_media(PassagemList passagens, VeiculoHashTable *veiculos, DistanciaList distancias);

/* registo de passagens */
int registar_passagem(PassagemList *list, int idSensor, int codVeiculo, struct tm data_tm, int milissegundos, int tipoRegisto);  // Adicionado milissegundos
void registar_passagem_interativo(PassagemList *list);

/* persistência */
void salvar_passagens(PassagemList list, const char* filename);

void exportPassagemToCSV(PassagemList list, const char *filename);

#endif /* PASSAGEM_H */