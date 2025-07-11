/**
 * @file utils.h
 * @brief Cabeçalho com funções utilitárias para ordenação e interface
 * @details Contém declarações para ordenação de arrays, paginação de resultados
 *          e funções auxiliares para leitura de input
 */

#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

/**
 * @brief Ordena um array usando o algoritmo de ordenação por fusão (merge sort)
 * @param arr Array genérico a ser ordenado (array de ponteiros void*)
 * @param l Índice inicial do subarray (normalmente 0)
 * @param r Índice final do subarray (tamanho-1)
 * @param compare Função de comparação personalizada
 * @note A função de comparação deve retornar:
 *       - Valor <0 se o primeiro elemento for menor
 *       - 0 se os elementos forem iguais
 *       - Valor >0 se o primeiro elemento for maior
 */
void merge_sort(void* arr[], int l, int r, int (*compare)(const void*, const void*));

/**
 * @brief Função de comparação de strings para usar com merge_sort
 * @param a Primeira string a comparar
 * @param b Segunda string a comparar
 * @return Resultado da comparação usando strcmp
 * @note Compatível com a assinatura exigida por merge_sort
 */
int comparar_strings(const char* a, const char* b);

/**
 * @brief Implementa um sistema de paginação para visualização de arrays
 * @param array Array genérico a ser mostrado
 * @param size Número total de elementos no array
 * @param dataPage Número de itens por página
 * @param fshow Função callback para mostrar um único elemento
 * @details Permite navegar entre páginas até o utilizador inserir -1
 *          Mostra o número da página atual e o total de páginas
 */
void paginacao(void **array, int size, int dataPage, void (*fshow)(void *data));

/**
 * @brief Lê um valor inteiro do utilizador com validação
 * @param mensagem Mensagem a mostrar ao utilizador
 * @param min Valor mínimo aceitável (inclusive)
 * @param max Valor máximo aceitável (inclusive)
 * @return Valor inteiro validado
 * @note Repete o pedido até receber um valor válido
 *       Limpa o buffer de input para evitar loops infinitos
 */
int lerInteiro(const char *mensagem, int min, int max);

/**
 * @brief Lê uma data e hora do utilizador
 * @return Valor time_t correspondente à data/hora inserida
 * @warning Não faz validação avançada dos valores inseridos
 * @note Converte para o formato time_t automaticamente
 *       Ajusta o ano (subtrai 1900) e mês (subtrai 1) conforme convenção da struct tm
 */
time_t lerDataHora();

#endif