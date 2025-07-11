#ifndef DONO_H
#define DONO_H

/**
 * @brief Representa um dono.
 */
typedef struct {
    int numContribuinte;  /**< Número de contribuinte. */
    char nome[100];       /**< Nome do dono. */
    char codPostal[20];   /**< Código postal. */
} Dono;

/**
 * @brief Nó da lista encadeada para tabela hash de donos.
 */
typedef struct DonoNode {
    Dono dono;                /**< Dados do dono. */
    struct DonoNode *next;    /**< Próximo nó. */
} DonoNode;

/**
 * @brief Estrutura de tabela hash para armazenar donos.
 */
typedef struct {
    DonoNode **buckets;   /**< Vetor de ponteiros para listas de colisão. */
    int size;             /**< Número de buckets. */
} DonoHashTable;

/**
 * @brief Mostra o subnome mais comum entre os nomes dos donos.
 * 
 * @param donos Tabela hash de donos.
 */
void mostrar_subnome_mais_comum(DonoHashTable *donos);

/**
 * @brief Cria uma nova tabela hash de donos.
 * 
 * @param size Tamanho da tabela hash.
 * @return Ponteiro para a tabela criada.
 */
DonoHashTable* criar_hash_table_donos(int size);

/**
 * @brief Libera a memória da tabela hash de donos.
 * 
 * @param table Tabela hash.
 */
void liberar_hash_table_donos(DonoHashTable* table);

/**
 * @brief Insere um dono na tabela hash.
 * 
 * @param table Tabela hash.
 * @param dono Dono a inserir.
 */
void inserir_dono(DonoHashTable* table, Dono dono);

/**
 * @brief Busca um dono pelo número de contribuinte.
 * 
 * @param table Tabela hash.
 * @param numContribuinte Número de contribuinte.
 * @return Ponteiro para o dono ou NULL se não existir.
 */
Dono* buscar_dono(DonoHashTable* table, int numContribuinte);

/**
 * @brief Carrega donos a partir de um arquivo.
 * 
 * @param table Tabela hash.
 * @param filename Nome do arquivo.
 */
void carregar_donos(DonoHashTable* table, const char* filename);

/**
 * @brief Calcula a memória usada pela tabela hash.
 * 
 * @param table Tabela hash.
 * @return Memória total em bytes.
 */
int calcular_memoria_donos(DonoHashTable* table);

/**
 * @brief Mostra as informações de um dono (usado na paginação).
 * 
 * @param dono Ponteiro genérico para Dono.
 */
void mostrarDono(void *dono);

/**
 * @brief Lista todos os donos em ordem alfabética.
 * 
 * @param table Tabela hash.
 */
void listar_donos_alfabetico(DonoHashTable* table);

/**
 * @brief Lista todos os donos por número de contribuinte.
 * 
 * @param table Tabela hash.
 */
void listar_donos_numContribuinte(DonoHashTable* table);

/**
 * @brief Regista um novo dono via terminal.
 * 
 * @param table Tabela hash.
 */
void registar_dono(DonoHashTable* table);

/**
 * @brief Salva todos os donos num arquivo de texto.
 * 
 * @param table Tabela hash.
 * @param filename Nome do arquivo.
 */
void salvar_donos(DonoHashTable* table, const char* filename);

/**
 * @brief Coleta todos os donos numa lista genérica.
 * 
 * @param table Tabela hash.
 * @param total Ponteiro para armazenar o total.
 * @return Lista de ponteiros para donos.
 */
void** coletar_donos(DonoHashTable* table, int* total);

/**
 * @brief Exporta todos os donos para um arquivo CSV.
 * 
 * @param table Tabela hash.
 * @param filename Nome do arquivo CSV.
 */
void exportDonoToCSV(DonoHashTable *table, const char *filename);

#endif
