#ifndef VEICULO_H
#define VEICULO_H

#include "../dono/dono.h"

typedef struct {
    char matricula[20];
    char marca[50];
    char modelo[50];
    int ano;
    Dono* dono;
    int codVeiculo;
} Veiculo;

typedef struct VeiculoNode {
    Veiculo veiculo;
    struct VeiculoNode *next;
} VeiculoNode;

typedef struct {
    VeiculoNode **buckets;
    int size;
} VeiculoHashTable;

VeiculoHashTable* criar_hash_table_veiculos(int size);
void libertar_hash_table_veiculos(VeiculoHashTable* table);
void inserir_veiculo(VeiculoHashTable* table, Veiculo veiculo);
void salvar_veiculos(VeiculoHashTable* table, const char* filename);
Veiculo* buscar_veiculo_matricula(VeiculoHashTable* table, const char* matricula);
Veiculo* buscar_veiculo_codigo(VeiculoHashTable* table, int codVeiculo);
void carregar_veiculos(VeiculoHashTable* table, DonoHashTable* donos, const char* filename);
int calcular_memoria_veiculos(VeiculoHashTable* table);
void listar_veiculos_ordenados(VeiculoHashTable* table, const char* criterio);
void registar_veiculo(VeiculoHashTable* table, DonoHashTable* donos);
int comparar_veiculos_matricula(const void* a, const void* b);

#endif