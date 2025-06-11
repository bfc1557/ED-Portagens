#include "dono.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../libs/utils.h"


DonoHashTable* criar_hash_table_donos(int size) {
    DonoHashTable* table = (DonoHashTable*)malloc(sizeof(DonoHashTable));
    table->size = size;
    table->buckets = (DonoNode**)calloc(size, sizeof(DonoNode*));
    return table;
}

void liberar_hash_table_donos(DonoHashTable* table) {
    for (int i = 0; i < table->size; i++) {
        DonoNode* current = table->buckets[i];
        while (current) {
            DonoNode* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(table->buckets);
    free(table);
}

int hash_dono(int numContribuinte, int size) {
    return numContribuinte % size;
}

void inserir_dono(DonoHashTable* table, Dono dono) {
    int index = hash_dono(dono.numContribuinte, table->size);
    DonoNode* newNode = (DonoNode*)malloc(sizeof(DonoNode));
    newNode->dono = dono;
    newNode->next = table->buckets[index];
    table->buckets[index] = newNode;
}

Dono* buscar_dono(DonoHashTable* table, int numContribuinte) {
    int index = hash_dono(numContribuinte, table->size);
    DonoNode* current = table->buckets[index];
    while (current) {
        if (current->dono.numContribuinte == numContribuinte) {
            return &current->dono;
        }
        current = current->next;
    }
    return NULL;
}

void carregar_donos(DonoHashTable* table, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir arquivo de donos");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        Dono dono;
        if (sscanf(line, "%d\t%99[^\t]\t%19s", &dono.numContribuinte, dono.nome, dono.codPostal) == 3) {
            inserir_dono(table, dono);
        }
    }
    fclose(file);
}

int calcular_memoria_donos(DonoHashTable* table) {
    int total = sizeof(DonoHashTable) + (table->size * sizeof(DonoNode*));
    
    for (int i = 0; i < table->size; i++) {
        DonoNode* current = table->buckets[i];
        while (current) {
            total += sizeof(DonoNode) + strlen(current->dono.nome) + strlen(current->dono.codPostal);
            current = current->next;
        }
    }
    return total;
}

void** coletar_donos(DonoHashTable* table, int* total) {
    *total = 0;

    // Contar o número total de donos
    for (int i = 0; i < table->size; i++) {
        DonoNode* current = table->buckets[i];
        while (current) {
            (*total)++;
            current = current->next;
        }
    }

    // Alocar array de void* em vez de Dono*
    void** array = (void**) malloc(*total * sizeof(Dono*));
    if (!array) return NULL;

    int index = 0;
    for (int i = 0; i < table->size; i++) {
        DonoNode* current = table->buckets[i];
        while (current) {
            array[index++] = (void*)&current->dono;
            current = current->next;
        }
    }

    return array;
}


int comparar_donos_nome(const void* a, const void* b) {
    const Dono* da = (const Dono*)a;
    const Dono* db = (const Dono*)b;
    return strcmp(da->nome, db->nome);
}

int comparar_donos_numContribuinte(const void* a, const void* b) {
    const Dono* da = (const Dono*)a;
    const Dono* db = (const Dono*)b;
    return da->numContribuinte - db->numContribuinte;
}

void listar_donos_alfabetico(DonoHashTable* table) {
    int total;
    Dono *dono;
    void** donos = coletar_donos(table, &total);
    merge_sort(donos, 0, total - 1, comparar_donos_nome);
    
    printf("\n=== Donos ordenados alfabeticamente ===\n");
    for (int i = 0; i < total; i++) {
        dono = (Dono*)donos[i];
        printf("%-8d %-30s %s\n", dono->numContribuinte, dono->nome, dono->codPostal);
    }
    free(donos);
}

void listar_donos_numContribuinte(DonoHashTable* table) {
    int total;
    Dono *dono;
    void **donos = coletar_donos(table, &total);
    merge_sort(donos, 0, total - 1, comparar_donos_numContribuinte);
    
    printf("\n=== Donos ordenados por número de contribuinte ===\n");
    for (int i = 0; i < total; i++) {
        dono = (Dono*)donos[i];
        printf("%-8d %-30s %s\n", dono->numContribuinte, dono->nome, dono->codPostal);
    }
    free(donos);
}

void registar_dono(DonoHashTable* table) {
    Dono novo;
    printf("\n=== Registar Novo Dono ===\n");
    printf("Número de contribuinte: ");
    scanf("%d", &novo.numContribuinte);
    
    if (buscar_dono(table, novo.numContribuinte)) {
        printf("Erro: Dono já existe!\n");
        return;
    }
    
    printf("Nome: ");
    scanf(" %[^\n]", novo.nome);
    printf("Código Postal: ");
    scanf(" %s", novo.codPostal);
    
    inserir_dono(table, novo);
    printf("Dono registado com sucesso!\n");
}