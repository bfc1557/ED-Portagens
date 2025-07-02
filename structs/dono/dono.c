#include "dono.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../../libs/utils.h"


typedef struct SubnomeItem {
    char subnome[64];
    int contador;
} SubnomeItem;

// Verifica se é uma preposição comum (para ignorar)
int eh_preposicao(const char *palavra) {
    return strcmp(palavra, "de") == 0 || strcmp(palavra, "da") == 0 ||
           strcmp(palavra, "do") == 0 || strcmp(palavra, "dos") == 0 ||
           strcmp(palavra, "das") == 0;
}

// Converte uma string para lowercase (para uniformizar)
void to_lower(char *str) {
    for (int i = 0; str[i]; i++)
        str[i] = tolower(str[i]);
}

void mostrar_subnome_mais_comum(DonoHashTable *donos)
{
    if (!donos)
        return;

    SubnomeItem *subnomes = NULL;
    int count = 0, capacity = 0;

    // Percorrer todos os donos (supondo que tens um array/lista ou iterador)
    for (int i = 0; i < donos->size; i++)
    {
        DonoNode *current = donos->buckets[i];
        while (current)
        {
            char nome_copy[256];
            strncpy(nome_copy, current->dono.nome, sizeof(nome_copy));
            nome_copy[sizeof(nome_copy) - 1] = '\0';

            // Tokenizar o nome
            char *token = strtok(nome_copy, " ");
            while (token)
            {
                char palavra[64];
                strncpy(palavra, token, sizeof(palavra));
                palavra[sizeof(palavra) - 1] = '\0';
                to_lower(palavra);

                if (!eh_preposicao(palavra))
                {
                    // Verificar se já existe
                    int existe = 0;
                    for (int j = 0; j < count; j++)
                    {
                        if (strcmp(subnomes[j].subnome, palavra) == 0)
                        {
                            subnomes[j].contador++;
                            existe = 1;
                            break;
                        }
                    }

                    if (!existe)
                    {
                        if (count >= capacity)
                        {
                            capacity = (capacity == 0) ? 16 : capacity * 2;
                            SubnomeItem *temp = realloc(subnomes, capacity * sizeof(SubnomeItem));
                            if (!temp)
                            {
                                perror("Erro ao alocar memória");
                                free(subnomes);
                                return;
                            }
                            subnomes = temp;
                        }

                        strcpy(subnomes[count].subnome, palavra);
                        subnomes[count].contador = 1;
                        count++;
                    }
                }

                token = strtok(NULL, " ");
            }

            current = current->next;
        }
    }

    // Encontrar o subnome mais comum
    int max_contador = 0;
    char mais_comum[64] = "";

    for (int i = 0; i < count; i++)
    {
        if (subnomes[i].contador > max_contador)
        {
            max_contador = subnomes[i].contador;
            strcpy(mais_comum, subnomes[i].subnome);
        }
    }

    // Mostrar resultado
    if (max_contador == 0)
    {
        printf("Nenhum subnome encontrado.\n");
    }
    else
    {
        printf("\n=== Subnome Mais Comum ===\n");
        printf("Subnome: %s\n", mais_comum);
        printf("Total: %d\n", max_contador);
    }

    free(subnomes);
}

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

void mostrarDono(void *ptrDono)
{
    Dono *dono;

    dono = (Dono*) ptrDono;

    printf("%-8d | %-100s | %s\n", dono->numContribuinte, dono->nome, dono->codPostal);
};


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
    
    paginacao(donos, total, 30, mostrarDono);
    free(donos);
}

void listar_donos_numContribuinte(DonoHashTable* table) {
    int total;
    Dono *dono;
    void **donos = coletar_donos(table, &total);
    merge_sort(donos, 0, total - 1, comparar_donos_numContribuinte);
    
    paginacao(donos, total, 30, mostrarDono);
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

void exportDonoToCSV(DonoHashTable *table, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Erro ao abrir arquivo");
        return;
    }

    fprintf(file, "numContribuinte,nome,codPostal\n");
    
    for (int i = 0; i < table->size; i++) {
        DonoNode *current = table->buckets[i];
        while (current != NULL) {
            fprintf(file, "%d,\"%s\",\"%s\"\n", 
                    current->dono.numContribuinte,
                    current->dono.nome,
                    current->dono.codPostal);
            current = current->next;
        }
    }
    
    fclose(file);
}

void salvar_donos(DonoHashTable* table, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Erro ao abrir arquivo de donos");
        return;
    }

    for (int i = 0; i < table->size; i++) {
        DonoNode* current = table->buckets[i];
        while (current != NULL) {
            fprintf(file, "%d\t%s\t%s\n",
                    current->dono.numContribuinte,
                    current->dono.nome,
                    current->dono.codPostal);
            current = current->next;
        }
    }

    fclose(file);
}
