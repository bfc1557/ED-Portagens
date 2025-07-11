#ifndef DISTANCIA_H
#define DISTANCIA_H

/**
 * @file distancia.h
 * @brief Declarações de tipos e funções para manipulação de distâncias entre sensores.
 */

/**
 * @struct Distancia
 * @brief Representa a distância entre dois sensores.
 * 
 * Contém os códigos dos dois sensores e a distância entre eles.
 */
typedef struct {
    int codSensor1; /**< Código do primeiro sensor. */
    int codSensor2; /**< Código do segundo sensor. */
    float distancia; /**< Distância em unidades definidas (ex.: km). */
} Distancia;

/**
 * @struct DistanciaNode
 * @brief Nó da lista ligada de distâncias.
 * 
 * Cada nó armazena uma distância e um ponteiro para o próximo nó.
 */
typedef struct DistanciaNode {
    Distancia distancia; /**< Estrutura de distância armazenada. */
    struct DistanciaNode *next; /**< Ponteiro para o próximo nó da lista. */
} DistanciaNode;

/**
 * @typedef DistanciaList
 * @brief Lista ligada de distâncias.
 * 
 * Ponteiro para o primeiro nó da lista de distâncias.
 */
typedef DistanciaNode* DistanciaList;

/**
 * @brief Cria uma lista de distâncias vazia.
 * 
 * @return Lista vazia (NULL).
 */
DistanciaList criar_lista_distancias();

/**
 * @brief Libera a memória de toda a lista de distâncias.
 * 
 * @param list Lista de distâncias a ser liberada.
 */
void liberar_lista_distancias(DistanciaList list);

/**
 * @brief Insere uma nova distância na lista.
 * 
 * @param list Ponteiro para o ponteiro da lista de distâncias.
 * @param distancia Estrutura de distância a ser inserida.
 */
void inserir_distancia(DistanciaList* list, Distancia distancia);

/**
 * @brief Busca a distância entre dois sensores na lista.
 * 
 * @param list Lista de distâncias.
 * @param codSensor1 Código do primeiro sensor.
 * @param codSensor2 Código do segundo sensor.
 * @return Distância entre os sensores ou -1.0f se não encontrada.
 */
float buscar_distancia(DistanciaList list, int codSensor1, int codSensor2);

/**
 * @brief Carrega distâncias de um arquivo para a lista.
 * 
 * O arquivo deve conter linhas com: codSensor1, codSensor2 e distância,
 * separados por tabulação (`\t`).
 * 
 * @param list Ponteiro para o ponteiro da lista de distâncias.
 * @param filename Nome do arquivo de entrada.
 */
void carregar_distancias(DistanciaList* list, const char* filename);

/**
 * @brief Calcula a quantidade de memória ocupada pela lista de distâncias.
 * 
 * @param list Lista de distâncias.
 * @return Total de memória (em bytes).
 */
int calcular_memoria_distancias(DistanciaList list);

/**
 * @brief Salva a lista de distâncias em um arquivo.
 * 
 * O arquivo conterá linhas no formato: codSensor1, codSensor2, distância
 * separados por tabulação (`\t`).
 * 
 * @param list Lista de distâncias.
 * @param filename Nome do arquivo de saída.
 */
void salvar_distancias(DistanciaList list, const char* filename);

/**
 * @brief Obtém a distância entre dois sensores.
 * 
 * Retorna a distância ou 0.0f se não encontrada.
 * 
 * @param distancias Lista de distâncias.
 * @param s1 Código do primeiro sensor.
 * @param s2 Código do segundo sensor.
 * @return Distância entre os sensores.
 */
float obter_distancia_entre_sensores(DistanciaList distancias, int s1, int s2);

/**
 * @brief Exporta a lista de distâncias em formato CSV.
 * 
 * O arquivo CSV gerado terá cabeçalho: codSensor1,codSensor2,distancia.
 * 
 * @param list Lista de distâncias.
 * @param filename Nome do arquivo CSV de saída.
 */
void exportDistanciaToCSV(DistanciaList list, const char *filename);

#endif
