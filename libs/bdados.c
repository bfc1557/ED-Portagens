#include "bdados.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void CalcularMemoria(BDados* bd) {
    bd->total_memoria = sizeof(BDados);
    bd->total_memoria += calcular_memoria_donos(bd->donos);
    printf("Memória utilizada pelos donos: %zu bytes\n", calcular_memoria_donos(bd->donos));
    bd->total_memoria += calcular_memoria_veiculos(bd->veiculos);
    printf("Memória utilizada pelos veículos: %zu bytes\n", calcular_memoria_veiculos(bd->veiculos));
    bd->total_memoria += calcular_memoria_sensores(bd->sensores);
    printf("Memória utilizada pelos sensores: %zu bytes\n", calcular_memoria_sensores(bd->sensores));
    bd->total_memoria += calcular_memoria_passagens(bd->passagens);~
    printf("Memória utilizada pelas passagens: %zu bytes\n", calcular_memoria_passagens(bd->passagens));
    bd->total_memoria += calcular_memoria_distancias(bd->distancias);
    printf("Memória utilizada pelas distâncias: %zu bytes\n", calcular_memoria_distancias(bd->distancias));
    
    printf("Memória total utilizada: %.2f MB\n", (double)bd->total_memoria / (1024.0 * 1024.0));
}

void CarregarDados(BDados* bd, const char* dir_dados) {
    char path[256];
    char base[256];

    snprintf(base, sizeof(base), "%s%s", dir_dados, dir_dados[strlen(dir_dados) - 1] == '/' ? "" : "/");

    snprintf(path, sizeof(path), "%s/donos.txt", base);
    carregar_donos(bd->donos, path);
    
    snprintf(path, sizeof(path), "%s/carros.txt", base);
    carregar_veiculos(bd->veiculos, bd->donos, path);
    
    snprintf(path, sizeof(path), "%s/sensores.txt", base);
    carregar_sensores(&bd->sensores, path);
    
    snprintf(path, sizeof(path), "%s/distancias.txt", base);
    carregar_distancias(&bd->distancias, path);
    
    snprintf(path, sizeof(path), "%s/passagem.txt", base);
    carregar_passagens(&bd->passagens, path, 100000);
    
    CalcularMemoria(bd);
}