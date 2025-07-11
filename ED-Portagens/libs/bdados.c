/**
 * @file bdados.c
 * @brief Módulo de gestão de dados do sistema (donos, veículos, sensores, passagens e distâncias)
 */

#include "bdados.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Inicializa a estrutura principal do sistema
 * 
 * Aloca memória para a estrutura BDados e inicializa todas as sub-estruturas:
 * - Tabelas hash para donos e veículos
 * - Listas ligadas para sensores, passagens e distâncias
 * 
 * @param nome Nome do sistema (para fins de log)
 * @return Ponteiro para a estrutura BDados inicializada
 * @warning Termina o programa com EXIT_FAILURE se falhar na alocação de memória
 */
BDados* InicializaPrograma(const char* nome) {
    BDados* bd = (BDados*)malloc(sizeof(BDados));
    if (!bd) {
        perror("Erro ao alocar memória para BDados");
        exit(EXIT_FAILURE);
    }
    
    bd->donos = criar_hash_table_donos(1000);
    bd->veiculos = criar_hash_table_veiculos(1000);
    bd->sensores = criar_lista_sensores();
    bd->passagens = criar_lista_passagens();
    bd->distancias = criar_lista_distancias();
    bd->total_memoria = sizeof(BDados);
    
    printf("Sistema %s inicializado\n", nome);
    return bd;
}

/**
 * @brief Libera toda a memória alocada pelo sistema
 * 
 * @param bd Ponteiro para a estrutura BDados a ser liberada
 * @note Função segura (verifica NULL pointer)
 */
void FinalizarPrograma(BDados* bd) {
    if (bd) {
        liberar_hash_table_donos(bd->donos);
        liberar_hash_table_veiculos(bd->veiculos);
        liberar_lista_sensores(bd->sensores);
        liberar_lista_passagens(bd->passagens);
        liberar_lista_distancias(bd->distancias);
        free(bd);
    }
}

/**
 * @brief Calcula e exibe estatísticas de uso de memória
 * 
 * @param bd Ponteiro para a estrutura BDados
 * @details Calcula:
 * 1. Memória usada por cada componente
 * 2. Memória total (em bytes e MB)
 */
void CalcularMemoria(BDados* bd) {
    bd->total_memoria = sizeof(BDados);
    bd->total_memoria += calcular_memoria_donos(bd->donos);
    printf("Memória utilizada pelos donos: %zu bytes\n", calcular_memoria_donos(bd->donos));
    bd->total_memoria += calcular_memoria_veiculos(bd->veiculos);
    printf("Memória utilizada pelos veículos: %zu bytes\n", calcular_memoria_veiculos(bd->veiculos));
    bd->total_memoria += calcular_memoria_sensores(bd->sensores);
    printf("Memória utilizada pelos sensores: %zu bytes\n", calcular_memoria_sensores(bd->sensores));
    bd->total_memoria += calcular_memoria_passagens(bd->passagens);
    printf("Memória utilizada pelas passagens: %zu bytes\n", calcular_memoria_passagens(bd->passagens));
    bd->total_memoria += calcular_memoria_distancias(bd->distancias);
    printf("Memória utilizada pelas distâncias: %zu bytes\n", calcular_memoria_distancias(bd->distancias));
    
    printf("Memória total utilizada: %.2f MB\n", (double)bd->total_memoria / (1024.0 * 1024.0));
}

/**
 * @brief Carrega dados de arquivos TXT para o sistema
 * 
 * @param bd Ponteiro para a estrutura BDados
 * @param dir_dados Diretório contendo os arquivos:
 * - donos.txt
 * - carros.txt
 * - sensores.txt
 * - distancias.txt
 * - passagem.txt (com limite de 1 milhão de registros)
 * @note Automaticamente chama CalcularMemoria() após carregar todos os dados
 */
void CarregarDados(BDados* bd, const char* dir_dados) {
    char path[256];
    char base[256];

    // Normaliza o path (adiciona '/' no final se necessário)
    snprintf(base, sizeof(base), "%s%s", dir_dados, dir_dados[strlen(dir_dados) - 1] == '/' ? "" : "/");

    // Carrega cada arquivo sequencialmente
    snprintf(path, sizeof(path), "%sdonos.txt", base);
    carregar_donos(bd->donos, path);
    
    snprintf(path, sizeof(path), "%scarros.txt", base);
    carregar_veiculos(bd->veiculos, bd->donos, path);
    
    snprintf(path, sizeof(path), "%ssensores.txt", base);
    carregar_sensores(&bd->sensores, path);
    
    snprintf(path, sizeof(path), "%sdistancias.txt", base);
    carregar_distancias(&bd->distancias, path);
    
    snprintf(path, sizeof(path), "%spassagem.txt", base);
    carregar_passagens(&bd->passagens, path, 1000000);
    
    CalcularMemoria(bd);
}

/**
 * @brief Salva os dados do sistema em arquivos TXT
 * 
 * @param bd Ponteiro para a estrutura BDados
 * @param dir_dados Diretório de destino para os arquivos
 * @note Mantém o mesmo formato de arquivos usado em CarregarDados()
 */
void GuardarDados(BDados* bd, const char* dir_dados) {
    char path[256];
    char base[256];

    snprintf(base, sizeof(base), "%s%s", dir_dados, dir_dados[strlen(dir_dados) - 1] == '/' ? "" : "/");

    snprintf(path, sizeof(path), "%sdonos.txt", base);
    salvar_donos(bd->donos, path);

    snprintf(path, sizeof(path), "%scarros.txt", base);
    salvar_veiculos(bd->veiculos, path);

    snprintf(path, sizeof(path), "%ssensores.txt", base);
    salvar_sensores(bd->sensores, path);

    snprintf(path, sizeof(path), "%sdistancias.txt", base);
    salvar_distancias(bd->distancias, path);

    snprintf(path, sizeof(path), "%spassagem.txt", base);
    salvar_passagens(bd->passagens, path);

    printf("Dados guardados com sucesso.\n");
}