/**
 * @file sensor.h
 * @brief Definições para gestão de sensores de portagem
 * @details Contém as estruturas de dados e protótipos de funções para manipulação de sensores
 */

#ifndef SENSOR_H
#define SENSOR_H

/**
 * @brief Estrutura que representa um sensor de portagem
 */
typedef struct {
    int codSensor;          /**< Código único de identificação do sensor */
    char designacao[100];   /**< Designação/descrição do sensor */
    char latitude[50];      /**< Coordenada de latitude (formato string) */
    char longitude[50];     /**< Coordenada de longitude (formato string) */
} Sensor;

/**
 * @brief Nó para lista ligada de sensores
 */
typedef struct SensorNode {
    Sensor sensor;          /**< Dados do sensor */
    struct SensorNode *next;/**< Ponteiro para o próximo nó na lista */
} SensorNode;

/**
 * @brief Tipo de dados para lista ligada de sensores
 */
typedef SensorNode* SensorList;

/* -------------------------------------------------------------------------- */
/* OPERAÇÕES BÁSICAS                                                          */
/* -------------------------------------------------------------------------- */

/**
 * @brief Cria uma lista vazia de sensores
 * @return Lista vazia (NULL)
 */
SensorList criar_lista_sensores();

/**
 * @brief Liberta toda a memória ocupada pela lista de sensores
 * @param list Lista de sensores a libertar
 */
void liberar_lista_sensores(SensorList list);

/**
 * @brief Insere um novo sensor na lista
 * @param list Ponteiro para a lista de sensores
 * @param sensor Dados do sensor a inserir
 * @note A inserção é feita no início da lista
 */
void inserir_sensor(SensorList* list, Sensor sensor);

/**
 * @brief Procura um sensor na lista pelo seu código
 * @param list Lista de sensores
 * @param codSensor Código do sensor a procurar
 * @return Ponteiro para o sensor encontrado ou NULL se não existir
 */
Sensor* buscar_sensor(SensorList list, int codSensor);

/* -------------------------------------------------------------------------- */
/* CARREGAMENTO E PERSISTÊNCIA                                                */
/* -------------------------------------------------------------------------- */

/**
 * @brief Carrega sensores a partir de um ficheiro
 * @param list Ponteiro para a lista de sensores
 * @param filename Nome do ficheiro a carregar
 * @note Formato esperado do ficheiro:
 *       codSensor\tdesignação\tlatitude\tlongitude
 */
void carregar_sensores(SensorList* list, const char* filename);

/**
 * @brief Guarda a lista de sensores num ficheiro
 * @param list Lista de sensores
 * @param filename Nome do ficheiro de destino
 * @note Mantém o mesmo formato usado no carregamento
 */
void salvar_sensores(SensorList list, const char* filename);

/* -------------------------------------------------------------------------- */
/* ESTATÍSTICAS E EXPORTAÇÃO                                                  */
/* -------------------------------------------------------------------------- */

/**
 * @brief Calcula a memória total utilizada pela lista de sensores
 * @param list Lista de sensores
 * @return Total de bytes utilizados
 * @note Inclui tamanho dos nós e strings alocadas
 */
int calcular_memoria_sensores(SensorList list);

/**
 * @brief Exporta a lista de sensores para formato CSV
 * @param list Lista de sensores
 * @param filename Nome do ficheiro CSV de destino
 * @note Formato do CSV:
 *       codSensor,designação,latitude,longitude
 *       As strings são envolvidas em aspas
 */
void exportSensorToCSV(SensorList list, const char *filename);

#endif /* SENSOR_H */