/**
 * @file veiculo.c
 * @brief Implementação das operações para gestão de veículos
 * @details Contém as funções para manipulação da tabela hash de veículos
 */

#include "veiculo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../libs/utils.h"

/* -------------------------------------------------------------------------- */
/* OPERAÇÕES BÁSICAS DA TABELA HASH                                           */
/* -------------------------------------------------------------------------- */

/**
 * @brief Cria uma nova tabela hash para veículos
 * @param size Tamanho da tabela hash
 * @return Ponteiro para a tabela hash criada
 * @note A tabela usa encadeamento para resolver colisões
 */
VeiculoHashTable* criar_hash_table_veiculos(int size) {
    VeiculoHashTable* table = (VeiculoHashTable*)malloc(sizeof(VeiculoHashTable));
    table->size = size;
    table->buckets = (VeiculoNode**)calloc(size, sizeof(VeiculoNode*));
    return table;
}

/**
 * @brief Liberta toda a memória ocupada pela tabela hash
 * @param table Tabela hash a libertar
 */
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

/**
 * @brief Função hash para veículos (usando matrícula)
 * @param matricula Matrícula do veículo
 * @param size Tamanho da tabela hash
 * @return Índice na tabela hash
 * @note Implementa o algoritmo djb2
 */
int hash_veiculo(const char* matricula, int size) {
    unsigned long hash = 5381;
    int c;
    while ((c = *matricula++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % size;
}

/**
 * @brief Insere um veículo na tabela hash
 * @param table Tabela hash
 * @param veiculo Dados do veículo a inserir
 */
void inserir_veiculo(VeiculoHashTable* table, Veiculo veiculo) {
    int index = hash_veiculo(veiculo.matricula, table->size);
    VeiculoNode* newNode = (VeiculoNode*)malloc(sizeof(VeiculoNode));
    newNode->veiculo = veiculo;
    newNode->next = table->buckets[index];
    table->buckets[index] = newNode;
}

/* -------------------------------------------------------------------------- */
/* OPERAÇÕES DE CONSULTA                                                      */
/* -------------------------------------------------------------------------- */

/**
 * @brief Procura um veículo pela matrícula
 * @param table Tabela hash
 * @param matricula Matrícula a procurar
 * @return Ponteiro para o veículo ou NULL se não encontrado
 */
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

/**
 * @brief Procura um veículo pelo código
 * @param table Tabela hash
 * @param codVeiculo Código a procurar
 * @return Ponteiro para o veículo ou NULL se não encontrado
 * @note Percorre toda a tabela (não usa hash)
 */
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

/* -------------------------------------------------------------------------- */
/* CARREGAMENTO E PERSISTÊNCIA                                                */
/* -------------------------------------------------------------------------- */

/**
 * @brief Carrega veículos a partir de um ficheiro
 * @param table Tabela hash
 * @param donos Tabela hash de donos (para associação)
 * @param filename Nome do ficheiro
 * @note Formato do ficheiro:
 *       matricula\tmarca\tmodelo\tano\tnumContribuinte\tcodVeiculo
 */
void carregar_veiculos(VeiculoHashTable* table, DonoHashTable* donos, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir ficheiro de veículos");
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
                fprintf(stderr, "Aviso: Dono não encontrado para veículo %s\n", veiculo.matricula);
            }
        }
    }
    fclose(file);
}

/**
 * @brief Guarda os veículos num ficheiro
 * @param table Tabela hash
 * @param filename Nome do ficheiro
 */
void salvar_veiculos(VeiculoHashTable* table, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Erro ao abrir ficheiro de veículos");
        return;
    }

    for (int i = 0; i < table->size; i++) {
        VeiculoNode* current = table->buckets[i];
        while (current != NULL) {
            fprintf(file, "%s\t%s\t%s\t%d\t%d\t%d\n",
                    current->veiculo.matricula,
                    current->veiculo.marca,
                    current->veiculo.modelo,
                    current->veiculo.ano,
                    current->veiculo.dono->numContribuinte, 
                    current->veiculo.codVeiculo);
            current = current->next;
        }
    }

    fclose(file);
}

/* -------------------------------------------------------------------------- */
/* ESTATÍSTICAS E EXPORTAÇÃO                                                  */
/* -------------------------------------------------------------------------- */

/**
 * @brief Calcula a memória utilizada pela tabela
 * @param table Tabela hash
 * @return Total de bytes utilizados
 */
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

/**
 * @brief Exporta os veículos para formato CSV
 * @param table Tabela hash
 * @param filename Nome do ficheiro CSV
 */
void exportVeiculoToCSV(VeiculoHashTable *table, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Erro ao abrir ficheiro CSV");
        return;
    }

    fprintf(file, "matricula,marca,modelo,ano,codVeiculo,numContribuinteDono\n");
    
    for (int i = 0; i < table->size; i++) {
        VeiculoNode *current = table->buckets[i];
        while (current != NULL) {
            fprintf(file, "\"%s\",\"%s\",\"%s\",%d,%d,%d\n", 
                    current->veiculo.matricula,
                    current->veiculo.marca,
                    current->veiculo.modelo,
                    current->veiculo.ano,
                    current->veiculo.codVeiculo,
                    current->veiculo.dono ? current->veiculo.dono->numContribuinte : 0);
            current = current->next;
        }
    }
    
    fclose(file);
}

/* -------------------------------------------------------------------------- */
/* FUNÇÕES AUXILIARES                                                         */
/* -------------------------------------------------------------------------- */

/**
 * @brief Coleta todos os veículos num array
 * @param table Tabela hash
 * @param total Ponteiro para armazenar o número total de veículos
 * @return Array de ponteiros para veículos
 */
void** coletar_veiculos(VeiculoHashTable* table, int* total) {
    *total = 0;
    for (int i = 0; i < table->size; i++) {
        VeiculoNode* current = table->buckets[i];
        while (current) {
            (*total)++;
            current = current->next;
        }
    }

    void** array = malloc(*total * sizeof(void*));
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

/**
 * @brief Função para mostrar um veículo
 * @param carro Ponteiro genérico para um veículo
 */
void mostrarCarro(void *carro) {
    Veiculo *ptrVeiculo = (Veiculo*) carro;
    printf("%-10d | %-10s | %-50s | %-50s | %-4d | %s\n", 
           ptrVeiculo->codVeiculo, 
           ptrVeiculo->matricula, 
           ptrVeiculo->marca, 
           ptrVeiculo->modelo, 
           ptrVeiculo->ano, 
           ptrVeiculo->dono ? ptrVeiculo->dono->nome : "N/A");
}

/* -------------------------------------------------------------------------- */
/* FUNÇÕES DE ORDENAÇÃO                                                       */
/* -------------------------------------------------------------------------- */

/**
 * @brief Compara veículos por matrícula
 * @param a Ponteiro para o primeiro veículo
 * @param b Ponteiro para o segundo veículo
 * @return Resultado da comparação (strcmp)
 */
int comparar_veiculos_matricula(const void* a, const void* b) {
    const Veiculo* va = (const Veiculo*)a;
    const Veiculo* vb = (const Veiculo*)b;
    return strcmp(va->matricula, vb->matricula);
}

/**
 * @brief Compara veículos por marca (e modelo se igual)
 * @param a Ponteiro para o primeiro veículo
 * @param b Ponteiro para o segundo veículo
 * @return Resultado da comparação
 */
int comparar_veiculos_marca(const void* a, const void* b) {
    const Veiculo* va = (const Veiculo*)a;
    const Veiculo* vb = (const Veiculo*)b;
    int cmp = strcmp(va->marca, vb->marca);
    return cmp == 0 ? strcmp(va->modelo, vb->modelo) : cmp;
}

/**
 * @brief Compara veículos por modelo (e marca se igual)
 * @param a Ponteiro para o primeiro veículo
 * @param b Ponteiro para o segundo veículo
 * @return Resultado da comparação
 */
int comparar_veiculos_modelo(const void* a, const void* b) {
    const Veiculo* va = (const Veiculo*)a;
    const Veiculo* vb = (const Veiculo*)b;
    int cmp = strcmp(va->modelo, vb->modelo);
    return cmp == 0 ? strcmp(va->marca, vb->marca) : cmp;
}

/* -------------------------------------------------------------------------- */
/* INTERFACE DO UTILIZADOR                                                    */
/* -------------------------------------------------------------------------- */

/**
 * @brief Lista veículos ordenados por um critério
 * @param table Tabela hash
 * @param criterio Critério de ordenação (matricula/marca/modelo)
 */
void listar_veiculos_ordenados(VeiculoHashTable* table, const char* criterio) {
    int total;
    void** veiculos = coletar_veiculos(table, &total);
    if (!veiculos) return;
    
    if (strcmp(criterio, "matricula") == 0) {
        merge_sort(veiculos, 0, total - 1, comparar_veiculos_matricula);
    } else if (strcmp(criterio, "marca") == 0) {
        merge_sort(veiculos, 0, total - 1, comparar_veiculos_marca);
    } else if (strcmp(criterio, "modelo") == 0) {
        merge_sort(veiculos, 0, total - 1, comparar_veiculos_modelo);
    } else {
        free(veiculos);
        return;
    }
    
    paginacao(veiculos, total, 30, mostrarCarro);
    free(veiculos);
}

/**
 * @brief Interface para registar novo veículo
 * @param table Tabela hash de veículos
 * @param donos Tabela hash de donos
 */
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