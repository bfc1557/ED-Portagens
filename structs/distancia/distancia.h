#ifndef DISTANCIA_H
#define DISTANCIA_H

typedef struct {
    int codSensor1;
    int codSensor2;
    float distancia;
} Distancia;

typedef struct DistanciaNode {
    Distancia distancia;
    struct DistanciaNode *next;
} DistanciaNode;

typedef DistanciaNode* DistanciaList;

DistanciaList criar_lista_distancias();
void liberar_lista_distancias(DistanciaList list);
void inserir_distancia(DistanciaList* list, Distancia distancia);
float buscar_distancia(DistanciaList list, int codSensor1, int codSensor2);
void carregar_distancias(DistanciaList* list, const char* filename);
int calcular_memoria_distancias(DistanciaList list);

#endif