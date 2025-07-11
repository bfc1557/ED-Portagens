/**
 * @file bdados.h
 * @brief Cabeçalho do módulo de gestão de dados do sistema
 * @details Contém a estrutura principal e funções para manipulação dos dados
 */

#ifndef BDADOS_H
#define BDADOS_H

#include "../structs/dono/dono.h"
#include "../structs/veiculo/veiculo.h"
#include "../structs/sensor/sensor.h"
#include "../structs/passagem/passagem.h"
#include "../structs/distancia/distancia.h"

/**
 * @brief Estrutura principal que agrega todos os dados do sistema
 * 
 * @struct BDados
 * @var donos - Tabela hash contendo todos os donos registrados
 * @var veiculos - Tabela hash contendo todos os veículos registrados
 * @var sensores - Lista ligada de sensores do sistema
 * @var passagens - Lista ligada de registros de passagem
 * @var distancias - Lista ligada de distâncias entre sensores
 * @var total_memoria - Memória total utilizada pelo sistema (em bytes)
 */
typedef struct {
    DonoHashTable *donos;        /**< Tabela hash de donos */
    VeiculoHashTable *veiculos;  /**< Tabela hash de veículos */
    SensorList sensores;         /**< Lista de sensores */
    PassagemList passagens;      /**< Lista de registros de passagem */
    DistanciaList distancias;    /**< Lista de distâncias entre sensores */
    double total_memoria;        /**< Memória total utilizada (em bytes) */
} BDados;

/**
 * @brief Inicializa a estrutura principal do sistema
 * @param nome Nome do sistema (para fins de log)
 * @return Ponteiro para a estrutura BDados inicializada
 */
BDados* InicializaPrograma(const char* nome);

/**
 * @brief Libera todos os recursos do sistema
 * @param bd Ponteiro para a estrutura BDados a ser liberada
 */
void FinalizarPrograma(BDados* bd);

/**
 * @brief Carrega dados de arquivos para o sistema
 * @param bd Ponteiro para a estrutura BDados
 * @param dir_dados Diretório contendo os arquivos de dados
 */
void CarregarDados(BDados* bd, const char* dir_dados);

/**
 * @brief Calcula estatísticas de uso de memória
 * @param bd Ponteiro para a estrutura BDados
 * @details Atualiza o campo total_memoria e exibe relatório
 */
void CalcularMemoria(BDados* bd);

/**
 * @brief Persiste os dados do sistema em arquivos
 * @param bd Ponteiro para a estrutura BDados
 * @param dir_dados Diretório de destino para os arquivos
 */
void GuardarDados(BDados* bd, const char* dir_dados);

#endif