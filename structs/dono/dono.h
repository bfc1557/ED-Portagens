#ifndef DONO_H
#define DONO_H

typedef struct {
    int numContribuinte;
    char nome[100];
    char codPostal[20];
} Dono;

typedef struct DonoNode {
    Dono dono;
    struct DonoNode *next;
} DonoNode;

typedef struct {
    DonoNode **buckets;
    int size;
} DonoHashTable;

DonoHashTable* criar_hash_table_donos(int size);
void liberar_hash_table_donos(DonoHashTable* table);
void inserir_dono(DonoHashTable* table, Dono dono);
void salvar_donos(DonoHashTable* table, const char* filename);
Dono* buscar_dono(DonoHashTable* table, int numContribuinte);
void carregar_donos(DonoHashTable* table, const char* filename);
int calcular_memoria_donos(DonoHashTable* table);
void mostrarDono(void *dono);
void listar_donos_alfabetico(DonoHashTable* table);
void listar_donos_numContribuinte(DonoHashTable* table);
void registar_dono(DonoHashTable* table);

void** coletar_donos(DonoHashTable* table, int* total);

#endif