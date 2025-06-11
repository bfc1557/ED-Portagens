#include "distancia.h"
#include <stdio.h>
#include <stdlib.h>

DistanciaList criar_lista_distancias() {
    return NULL;
}

void liberar_lista_distancias(DistanciaList list) {
    DistanciaNode* current = list;
    while (current) {
        DistanciaNode* temp = current;
        current = current->next;
        free(temp);
    }
}

void inserir_distancia(DistanciaList* list, Distancia distancia) {
    DistanciaNode* newNode = (DistanciaNode*)malloc(sizeof(DistanciaNode));
    newNode->distancia = distancia;
    newNode->next = *list;
    *list = newNode;
}

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

int calcular_memoria_distancias(DistanciaList list) {
    int total = 0;
    DistanciaNode* current = list;
    while (current) {
        total += sizeof(DistanciaNode);
        current = current->next;
    }
    return total;
}