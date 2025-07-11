/**
 * @file distancia.c
 * @brief Implementação de operações para manipulação de listas de distâncias entre sensores.
 */

#include "distancia.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Cria uma lista de distâncias vazia.
 * 
 * @return Ponteiro para a lista de distâncias (NULL, lista vazia).
 */
DistanciaList criar_lista_distancias() {
    return NULL;
}

/**
 * @brief Libera toda a memória alocada para a lista de distâncias.
 * 
 * @param list Ponteiro para a lista de distâncias.
 */
void liberar_lista_distancias(DistanciaList list) {
    DistanciaNode* current = list;
    while (current) {
        DistanciaNode* temp = current;
        current = current->next;
        free(temp);
    }
}

/**
 * @brief Insere uma nova distância na lista.
 * 
 * @param list Ponteiro para o ponteiro da lista de distâncias.
 * @param distancia Estrutura de distância a ser inserida.
 */
void inserir_distancia(DistanciaList* list, Distancia distancia) {
    DistanciaNode* newNode = (DistanciaNode*)malloc(sizeof(DistanciaNode));
    newNode->distancia = distancia;
    newNode->next = *list;
    *list = newNode;
}

/**
 * @brief Busca a distância entre dois sensores na lista.
 * 
 * @param list Lista de distâncias.
 * @param codSensor1 Código do primeiro sensor.
 * @param codSensor2 Código do segundo sensor.
 * @return Distância entre os sensores, ou -1.0f se não encontrada.
 */
float buscar_distancia(DistanciaList list, int codSensor1, int codSensor2) {
    DistanciaNode* current = list;
    while (current) {
        if ((current->distancia.codSensor1 == codSensor1 && 
             current->distancia.codSensor2 == codSensor2) ||
            (current->distancia.codSensor1 == codSensor2 && 
             current->distancia.codSensor2 == codSensor1)) {
            return current->distancia.distancia;
        }
        current = current->next;
    }
    return -1.0f; // Indica que não encontrou
}

/**
 * @brief Carrega distâncias de um arquivo para a lista.
 * 
 * Cada linha do arquivo deve conter: codSensor1, codSensor2, distância
 * separados por tabulação (`\t`).
 * 
 * @param list Ponteiro para o ponteiro da lista de distâncias.
 * @param filename Nome do arquivo a ser lido.
 */
void carregar_distancias(DistanciaList* list, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir arquivo de distâncias");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        Distancia distancia;
        if (sscanf(line, "%d\t%d\t%f", &distancia.codSensor1, 
                  &distancia.codSensor2, &distancia.distancia) == 3) {
            inserir_distancia(list, distancia);
        }
    }
    fclose(file);
}

/**
 * @brief Calcula a memória ocupada pela lista de distâncias.
 * 
 * @param list Lista de distâncias.
 * @return Total de memória (em bytes) ocupada pela lista.
 */
int calcular_memoria_distancias(DistanciaList list) {
    int total = 0;
    DistanciaNode* current = list;
    while (current) {
        total += sizeof(DistanciaNode);
        current = current->next;
    }
    return total;
}

/**
 * @brief Salva a lista de distâncias num arquivo.
 * 
 * Cada linha do arquivo gerado conterá: codSensor1, codSensor2, distância
 * separados por tabulação (`\t`).
 * 
 * @param list Lista de distâncias.
 * @param filename Nome do arquivo de saída.
 */
void salvar_distancias(DistanciaList list, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Erro ao abrir arquivo de distâncias");
        return;
    }

    DistanciaNode* current = list;
    while (current != NULL) {
        fprintf(file, "%d\t%d\t%.1f\n",
                current->distancia.codSensor1,
                current->distancia.codSensor2,
                current->distancia.distancia);
        current = current->next;
    }

    fclose(file);
}

/**
 * @brief Obtém a distância entre dois sensores.
 * 
 * Funciona como `buscar_distancia()`, mas retorna 0.0f caso não encontre.
 * 
 * @param distancias Lista de distâncias.
 * @param s1 Código do primeiro sensor.
 * @param s2 Código do segundo sensor.
 * @return Distância entre os sensores, ou 0.0f se não encontrada.
 */
float obter_distancia_entre_sensores(DistanciaList distancias, int s1, int s2) {
    for (DistanciaNode* node = distancias; node != NULL; node = node->next) {
        if ((node->distancia.codSensor1 == s1 && node->distancia.codSensor2 == s2) ||
            (node->distancia.codSensor1 == s2 && node->distancia.codSensor2 == s1)) {
            return node->distancia.distancia;
        }
    }
    return 0.0f;  // Se não encontrar distância, considera 0 km
}

/**
 * @brief Exporta a lista de distâncias em formato CSV.
 * 
 * Gera um arquivo CSV com cabeçalho: `codSensor1,codSensor2,distancia`
 * e cada linha com os valores separados por vírgula.
 * 
 * @param list Lista de distâncias.
 * @param filename Nome do arquivo CSV de saída.
 */
void exportDistanciaToCSV(DistanciaList list, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Erro ao abrir arquivo");
        return;
    }

    fprintf(file, "codSensor1,codSensor2,distancia\n");
    
    DistanciaNode *current = list;
    while (current != NULL) {
        fprintf(file, "%d,%d,%.2f\n", 
                current->distancia.codSensor1,
                current->distancia.codSensor2,
                current->distancia.distancia);
        current = current->next;
    }
    
    fclose(file);
}
