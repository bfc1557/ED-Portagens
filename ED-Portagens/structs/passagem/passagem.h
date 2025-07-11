/**
 * @file passagem.h
 * @brief Definições para gestão de registos de passagem
 * @details Contém as estruturas e funções para manipulação de passagens de veículos
 */

#ifndef PASSAGEM_H
#define PASSAGEM_H

#include <time.h>
#include "../veiculo/veiculo.h"
#include "../distancia/distancia.h"
#include "../../libs/utils.h"

/**
 * @brief Estrutura que representa um registo de passagem
 */
typedef struct {
    int idSensor;           /**< Identificador do sensor que registou a passagem */
    int codVeiculo;         /**< Código do veículo que efetuou a passagem */
    time_t data;            /**< Data e hora da passagem (formato time_t) */
    int milissegundos;      /**< Milissegundos do registo (precisão adicional) */
    int tipoRegisto;        /**< Tipo de registo (0 = entrada, 1 = saída) */
} Passagem;

/**
 * @brief Nó para lista ligada de passagens
 */
typedef struct PassagemNode {
    Passagem passagem;          /**< Dados da passagem */
    struct PassagemNode *next;   /**< Ponteiro para o próximo nó */
} PassagemNode;

/** @brief Tipo de dados para lista ligada de passagens */
typedef PassagemNode* PassagemList;

/* ------------------------ */
/* OPERAÇÕES BÁSICAS        */
/* ------------------------ */

/**
 * @brief Cria uma lista vazia de passagens
 * @return Lista vazia inicializada
 */
PassagemList criar_lista_passagens(void);

/**
 * @brief Liberta toda a memória ocupada pela lista de passagens
 * @param list Lista a ser libertada
 */
void liberar_lista_passagens(PassagemList list);

/**
 * @brief Insere uma nova passagem na lista
 * @param list Ponteiro para a lista de passagens
 * @param passagem Dados da passagem a inserir
 */
void inserir_passagem(PassagemList *list, Passagem passagem);

/* ------------------------ */
/* CARREGAMENTO E ESTATÍSTICAS */
/* ------------------------ */

/**
 * @brief Carrega passagens a partir de um ficheiro
 * @param list Ponteiro para a lista de passagens
 * @param filename Nome do ficheiro a carregar
 * @param batch_size Tamanho do lote para carregamento (otimização de memória)
 */
void carregar_passagens(PassagemList *list, const char *filename, int batch_size);

/**
 * @brief Calcula a memória total ocupada pela lista de passagens
 * @param list Lista de passagens
 * @return Total de bytes utilizados
 */
int calcular_memoria_passagens(PassagemList list);

/* ------------------------ */
/* LISTAGENS E CONSULTAS    */
/* ------------------------ */

/**
 * @brief Lista passagens ocorridas num determinado período
 * @param list Lista de passagens
 * @param inicio Data/hora inicial do período
 * @param fim Data/hora final do período
 */
void listar_passagens_periodo(PassagemList list, time_t inicio, time_t fim);

/**
 * @brief Lista veículos que circularam num período
 * @param passagens Lista de passagens
 * @param veiculos Tabela hash de veículos
 * @param inicio Data/hora inicial
 * @param fim Data/hora final
 */
void listar_veiculos_periodo(PassagemList passagens, VeiculoHashTable* veiculos, time_t inicio, time_t fim);

/**
 * @brief Lista ranking de veículos por quilómetros percorridos
 * @param passagens Lista de passagens
 * @param veiculos Tabela hash de veículos
 * @param distancias Lista de distâncias entre sensores
 * @param inicio Data/hora inicial
 * @param fim Data/hora final
 */
void listar_ranking_circulacao(PassagemList passagens, VeiculoHashTable* veiculos, DistanciaList distancias, time_t inicio, time_t fim);

/**
 * @brief Lista ranking de marcas por quilómetros percorridos
 * @param passagens Lista de passagens
 * @param veiculos Tabela hash de veículos
 * @param distancias Lista de distâncias entre sensores
 * @param inicio Data/hora inicial
 * @param fim Data/hora final
 */
void listar_ranking_marca(PassagemList passagens, VeiculoHashTable* veiculos, DistanciaList distancias, time_t inicio, time_t fim);

/**
 * @brief Lista infrações (velocidade > 120 km/h)
 * @param passagens Lista de passagens
 * @param veiculos Tabela hash de veículos
 * @param distancias Lista de distâncias entre sensores
 * @param inicio Data/hora inicial
 * @param fim Data/hora final
 */
void listar_infracoes(PassagemList passagens, VeiculoHashTable *veiculos, DistanciaList distancias, time_t inicio, time_t fim);

/**
 * @brief Lista ranking de veículos por número de infrações
 * @param passagens Lista de passagens
 * @param veiculos Tabela hash de veículos
 * @param distancias Lista de distâncias entre sensores
 * @param inicio Data/hora inicial
 * @param fim Data/hora final
 */
void listar_ranking_infracoes(PassagemList passagens, VeiculoHashTable *veiculos, DistanciaList distancias, time_t inicio, time_t fim);

/**
 * @brief Identifica a marca com maior velocidade média
 * @param passagens Lista de passagens
 * @param veiculos Tabela hash de veículos
 * @param distancias Lista de distâncias entre sensores
 */
void listar_marca_maior_velocidade_media(PassagemList passagens, VeiculoHashTable *veiculos, DistanciaList distancias);

/**
 * @brief Identifica o dono com maior velocidade média
 * @param passagens Lista de passagens
 * @param veiculos Tabela hash de veículos
 * @param distancias Lista de distâncias entre sensores
 */
void listar_dono_maior_velocidade_media(PassagemList passagens, VeiculoHashTable *veiculos, DistanciaList distancias);

/**
 * @brief Identifica o veículo mais rápido e seu condutor
 * @param passagens Lista de passagens
 * @param veiculos Tabela hash de veículos
 * @param distancias Lista de distâncias entre sensores
 */
void mostrar_veiculo_mais_rapido(PassagemList passagens, VeiculoHashTable *veiculos, DistanciaList distancias);

/* ------------------------ */
/* REGISTO DE PASSAGENS     */
/* ------------------------ */

/**
 * @brief Regista uma nova passagem
 * @param list Ponteiro para a lista de passagens
 * @param idSensor Identificador do sensor
 * @param codVeiculo Código do veículo
 * @param data_tm Estrutura tm com data/hora
 * @param milissegundos Milissegundos do registo
 * @param tipoRegisto Tipo de registo (0=entrada, 1=saída)
 * @return 1 em caso de sucesso, 0 em caso de erro
 */
int registar_passagem(PassagemList *list, int idSensor, int codVeiculo, struct tm data_tm, int milissegundos, int tipoRegisto);

/**
 * @brief Interface interativa para registar passagem
 * @param list Ponteiro para a lista de passagens
 */
void registar_passagem_interativo(PassagemList *list);

/* ------------------------ */
/* PERSISTÊNCIA DE DADOS    */
/* ------------------------ */

/**
 * @brief Guarda a lista de passagens num ficheiro
 * @param list Lista de passagens
 * @param filename Nome do ficheiro de destino
 */
void salvar_passagens(PassagemList list, const char* filename);

/**
 * @brief Exporta passagens para formato CSV
 * @param list Lista de passagens
 * @param filename Nome do ficheiro CSV
 */
void exportPassagemToCSV(PassagemList list, const char *filename);

#endif /* PASSAGEM_H */