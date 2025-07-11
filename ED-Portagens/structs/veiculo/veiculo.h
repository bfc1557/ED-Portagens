#ifndef VEICULO_H
#define VEICULO_H

#include "../dono/dono.h"

/**
 * \file veiculo.h
 * \brief Definições e funções para manipulação de veículos.
 */

/**
 * \struct Veiculo
 * \brief Estrutura que representa um veículo.
 */
typedef struct {
    char matricula[20];   /**< Matrícula do veículo */
    char marca[50];       /**< Marca do veículo */
    char modelo[50];      /**< Modelo do veículo */
    int ano;              /**< Ano de fabrico */
    Dono* dono;           /**< Ponteiro para o dono do veículo */
    int codVeiculo;       /**< Código interno do veículo */
} Veiculo;

/**
 * \struct VeiculoNode
 * \brief Nó de lista ligada para armazenar veículos.
 */
typedef struct VeiculoNode {
    Veiculo veiculo;           /**< Dados do veículo */
    struct VeiculoNode *next;  /**< Próximo nó */
} VeiculoNode;

/**
 * \struct VeiculoHashTable
 * \brief Estrutura para tabela hash de veículos.
 */
typedef struct {
    VeiculoNode **buckets; /**< Vetor de listas de veículos */
    int size;              /**< Tamanho da tabela hash */
} VeiculoHashTable;

/**
 * \brief Cria uma tabela hash de veículos.
 * \param size Tamanho da tabela.
 * \return Ponteiro para a tabela criada.
 */
VeiculoHashTable* criar_hash_table_veiculos(int size);

/**
 * \brief Liberta a memória da tabela hash de veículos.
 * \param table Ponteiro para a tabela.
 */
void liberar_hash_table_veiculos(VeiculoHashTable* table);

/**
 * \brief Insere um veículo na tabela hash.
 * \param table Ponteiro para a tabela.
 * \param veiculo Veículo a inserir.
 */
void inserir_veiculo(VeiculoHashTable* table, Veiculo veiculo);

/**
 * \brief Procura um veículo pela matrícula.
 * \param table Ponteiro para a tabela.
 * \param matricula Matrícula a procurar.
 * \return Ponteiro para o veículo ou NULL.
 */
Veiculo* buscar_veiculo_matricula(VeiculoHashTable* table, const char* matricula);

/**
 * \brief Procura um veículo pelo código.
 * \param table Ponteiro para a tabela.
 * \param codVeiculo Código do veículo.
 * \return Ponteiro para o veículo ou NULL.
 */
Veiculo* buscar_veiculo_codigo(VeiculoHashTable* table, int codVeiculo);

/**
 * \brief Carrega veículos de um ficheiro.
 * \param table Ponteiro para a tabela.
 * \param donos Tabela hash de donos.
 * \param filename Nome do ficheiro.
 */
void carregar_veiculos(VeiculoHashTable* table, DonoHashTable* donos, const char* filename);

/**
 * \brief Calcula a memória ocupada pela tabela de veículos.
 * \param table Ponteiro para a tabela.
 * \return Memória em bytes.
 */
int calcular_memoria_veiculos(VeiculoHashTable* table);

/**
 * \brief Lista veículos ordenados por critério.
 * \param table Ponteiro para a tabela.
 * \param criterio Critério de ordenação.
 */
void listar_veiculos_ordenados(VeiculoHashTable* table, const char* criterio);

/**
 * \brief Regista um novo veículo.
 * \param table Ponteiro para a tabela.
 * \param donos Tabela hash de donos.
 */
void registar_veiculo(VeiculoHashTable* table, DonoHashTable* donos);

/**
 * \brief Compara dois veículos por matrícula.
 * \param a Ponteiro para veículo A.
 * \param b Ponteiro para veículo B.
 * \return <0, 0 ou >0.
 */
int comparar_veiculos_matricula(const void* a, const void* b);

/**
 * \brief Salva os veículos num ficheiro.
 * \param table Ponteiro para a tabela.
 * \param filename Nome do ficheiro.
 */
void salvar_veiculos(VeiculoHashTable* table, const char* filename);

/**
 * \brief Mostra os dados de um veículo.
 * \param carro Ponteiro para o veículo.
 */
void mostrarCarro(void *carro);

/**
 * \brief Exporta os veículos para um ficheiro CSV.
 * \param table Ponteiro para a tabela.
 * \param filename Nome do ficheiro CSV.
 */
void exportVeiculoToCSV(VeiculoHashTable *table, const char *filename);

#endif