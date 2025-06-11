#include "veiculo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../libs/utils.h"

VeiculoHashTable* criar_hash_table_veiculos(int size) {
    VeiculoHashTable* table = (VeiculoHashTable*)malloc(sizeof(VeiculoHashTable));
    table->size = size;
    table->buckets = (VeiculoNode**)calloc(size, sizeof(VeiculoNode*));
    return table;
}

void liberar_hash_table_veiculos(VeiculoHashTable* table) {
    for (int i = 0; i < table->size; i++) {
        VeiculoNode* current = table->buckets[i];
        while (current) {
            VeiculoNode* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(table->buckets);
    free(table);
}

int hash_veiculo(const char* matricula, int size) {
    unsigned long hash = 5381;
    int c;
    while ((c = *matricula++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % size;
}

void inserir_veiculo(VeiculoHashTable* table, Veiculo veiculo) {
    int index = hash_veiculo(veiculo.matricula, table->size);
    VeiculoNode* newNode = (VeiculoNode*)malloc(sizeof(VeiculoNode));
    newNode->veiculo = veiculo;
    newNode->next = table->buckets[index];
    table->buckets[index] = newNode;
}

Veiculo* buscar_veiculo_matricula(VeiculoHashTable* table, const char* matricula) {
    int index = hash_veiculo(matricula, table->size);
    VeiculoNode* current = table->buckets[index];
    while (current) {
        if (strcmp(current->veiculo.matricula, matricula) == 0) {
            return &current->veiculo;
        }
        current = current->next;
    }
    return NULL;
}

Veiculo* buscar_veiculo_codigo(VeiculoHashTable* table, int codVeiculo) {
    for (int i = 0; i < table->size; i++) {
        VeiculoNode* current = table->buckets[i];
        while (current) {
            if (current->veiculo.codVeiculo == codVeiculo) {
                return &current->veiculo;
            }
            current = current->next;
        }
    }
    return NULL;
}

void carregar_veiculos(VeiculoHashTable* table, DonoHashTable* donos, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir arquivo de veículos");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        Veiculo veiculo;
        int numContribuinte;
        
        if (sscanf(line, "%19[^\t]\t%49[^\t]\t%49[^\t]\t%d\t%d\t%d",
                  veiculo.matricula, veiculo.marca, veiculo.modelo,
                  &veiculo.ano, &numContribuinte, &veiculo.codVeiculo) == 6) {
            
            veiculo.dono = buscar_dono(donos, numContribuinte);
            if (veiculo.dono) {
                inserir_veiculo(table, veiculo);
            } else {
                fprintf(stderr, "Dono não encontrado para veículo %s\n", veiculo.matricula);
            }
        }
    }
    fclose(file);
}

int calcular_memoria_veiculos(VeiculoHashTable* table) {
    int total = sizeof(VeiculoHashTable) + (table->size * sizeof(VeiculoNode*));
    
    for (int i = 0; i < table->size; i++) {
        VeiculoNode* current = table->buckets[i];
        while (current) {
            total += sizeof(VeiculoNode) + strlen(current->veiculo.matricula) +
                     strlen(current->veiculo.marca) + strlen(current->veiculo.modelo);
            current = current->next;
        }
    }
    return total;
}

Veiculo** coletar_veiculos(VeiculoHashTable* table, int* total) {
    *total = 0;
    for (int i = 0; i < table->size; i++) {
        VeiculoNode* current = table->buckets[i];
        while (current) {
            (*total)++;
            current = current->next;
        }
    }
    
    Veiculo** array = (Veiculo**)malloc(*total * sizeof(Veiculo*));
    int index = 0;
    for (int i = 0; i < table->size; i++) {
        VeiculoNode* current = table->buckets[i];
        while (current) {
            array[index++] = &current->veiculo;
            current = current->next;
        }
    }
    return array;
}

int comparar_veiculos_matricula(const void* a, const void* b) {
    const Veiculo* va = *(const Veiculo**)a;
    const Veiculo* vb = *(const Veiculo**)b;
    return strcmp(va->matricula, vb->matricula);
}

int comparar_veiculos_marca(const void* a, const void* b) {
    const Veiculo* va = *(const Veiculo**)a;
    const Veiculo* vb = *(const Veiculo**)b;
    int cmp = strcmp(va->marca, vb->marca);
    return cmp == 0 ? strcmp(va->modelo, vb->modelo) : cmp;
}

int comparar_veiculos_modelo(const void* a, const void* b) {
    const Veiculo* va = *(const Veiculo**)a;
    const Veiculo* vb = *(const Veiculo**)b;
    int cmp = strcmp(va->modelo, vb->modelo);
    return cmp == 0 ? strcmp(va->marca, vb->marca) : cmp;
}

void listar_veiculos_ordenados(VeiculoHashTable* table, const char* criterio) {
    int total;
    Veiculo** veiculos = coletar_veiculos(table, &total);
    if (!veiculos) return;
    
    if (strcmp(criterio, "matricula") == 0) {
        merge_sort((void**)veiculos, 0, total - 1, comparar_veiculos_matricula);
        printf("\n=== Veículos ordenados por matrícula ===\n");
    } else if (strcmp(criterio, "marca") == 0) {
        merge_sort((void**)veiculos, 0, total - 1, comparar_veiculos_marca);
        printf("\n=== Veículos ordenados por marca ===\n");
    } else if (strcmp(criterio, "modelo") == 0) {
        merge_sort((void**)veiculos, 0, total - 1, comparar_veiculos_modelo);
        printf("\n=== Veículos ordenados por modelo ===\n");
    } else {
        free(veiculos);
        return;
    }
    
    printf("%-10s %-15s %-15s %-4s %s\n", "Matrícula", "Marca", "Modelo", "Ano", "Dono");
    for (int i = 0; i < total; i++) {
        printf("%-10s %-15s %-15s %-4d %s\n", 
               veiculos[i]->matricula, 
               veiculos[i]->marca, 
               veiculos[i]->modelo, 
               veiculos[i]->ano,
               veiculos[i]->dono ? veiculos[i]->dono->nome : "N/A");
    }
    free(veiculos);
}

void registar_veiculo(VeiculoHashTable* table, DonoHashTable* donos) {
    Veiculo novo;
    printf("\n=== Registar Novo Veículo ===\n");
    printf("Matrícula: ");
    scanf(" %19[^\n]", novo.matricula);
    
    if (buscar_veiculo_matricula(table, novo.matricula)) {
        printf("Erro: Veículo já existe!\n");
        return;
    }
    
    printf("Marca: ");
    scanf(" %49[^\n]", novo.marca);
    printf("Modelo: ");
    scanf(" %49[^\n]", novo.modelo);
    printf("Ano: ");
    scanf("%d", &novo.ano);
    
    int numContribuinte;
    printf("Número de contribuinte do dono: ");
    scanf("%d", &numContribuinte);
    novo.dono = buscar_dono(donos, numContribuinte);
    
    if (!novo.dono) {
        printf("Erro: Dono não encontrado!\n");
        return;
    }
    
    static int ultimoCodigo = 0;
    novo.codVeiculo = ++ultimoCodigo;
    
    inserir_veiculo(table, novo);
    printf("Veículo registado com sucesso! Código: %d\n", novo.codVeiculo);
}