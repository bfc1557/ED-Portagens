/**
 * @file menu.c
 * @brief Implementação dos menus do sistema de gestão de portagens
 * @details Contém todas as funções de interface com o utilizador
 */

#include "menu.h"
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * @brief Apresenta o menu principal e obtém a escolha do utilizador
 * @return Opção selecionada (0-6)
 * @note As opções correspondem a:
 *       1-Donos, 2-Veículos, 3-Passagens, 4-Consultas,
 *       5-Estatísticas, 6-Exportar, 0-Sair
 */
int MenuPrincipal() {
    system("clear || cls");
    printf("\n=== Menu Principal ===\n");
    printf("1. Donos\n");
    printf("2. Veiculos\n");
    printf("3. Passagens\n");
    printf("4. Consultas\n");
    printf("5. Estatisticas\n");
    printf("6. Exportar\n");
    printf("0. Sair\n");
    
    return lerInteiro("Escolha: ", 0, 6);
}

/**
 * @brief Menu de gestão de donos de veículos
 * @param bd Ponteiro para a base de dados
 */
void MenuDonos(BDados* bd) {
    system("clear || cls");
    int opcao;
    do {
        printf("\n=== Menu Donos ===\n");
        printf("1. Registar dono\n");
        printf("2. Listar ordenados alfabeticamente\n");
        printf("3. Listar ordenados por número contribuinte\n");
        printf("0. Voltar\n");
        opcao = lerInteiro("Escolha: ", 0, 3);
        
        switch(opcao) {
            case 1: 
                registar_dono(bd->donos); 
                break;
            case 2: 
                listar_donos_alfabetico(bd->donos); 
                break;
            case 3: 
                listar_donos_numContribuinte(bd->donos); 
                break;
            case 0: 
                break;
            default: 
                printf("Opcao invalida!\n");
        }
    } while (opcao != 0);
}

/**
 * @brief Menu de gestão de veículos
 * @param bd Ponteiro para a base de dados
 */
void MenuVeiculos(BDados* bd) {
    system("clear || cls");
    int opcao;
    do {
        printf("\n=== Menu Veiculos ===\n");
        printf("1. Registar veiculo\n");
        printf("2. Listar por matricula\n");
        printf("3. Listar por marca\n");
        printf("4. Listar por modelo\n");
        printf("0. Voltar\n");
        opcao = lerInteiro("Escolha: ", 0, 4);
        
        switch(opcao) {
            case 1: 
                registar_veiculo(bd->veiculos, bd->donos); 
                break;
            case 2: 
                listar_veiculos_ordenados(bd->veiculos, "matricula"); 
                break;
            case 3: 
                listar_veiculos_ordenados(bd->veiculos, "marca"); 
                break;
            case 4: 
                listar_veiculos_ordenados(bd->veiculos, "modelo"); 
                break;
            case 0: 
                break;
            default: 
                printf("Opcao invalida!\n");
        }
    } while (opcao != 0);
}

/**
 * @brief Menu de gestão de passagens
 * @param bd Ponteiro para a base de dados
 */
void MenuPassagens(BDados* bd) {
    system("clear || cls");
    int opcao;
    do {
        printf("\n=== Menu Passagens ===\n");
        printf("1. Registar Passagem\n");
        printf("0. Voltar\n");
        opcao = lerInteiro("Escolha: ", 0, 1);
        
        switch(opcao) {
            case 1: 
                registar_passagem_interativo(&bd->passagens); 
                break;
            case 0: 
                break;
            default: 
                printf("Opcao invalida!\n");
        }
    } while (opcao != 0);
}

/**
 * @brief Menu de consultas avançadas
 * @param bd Ponteiro para a base de dados
 * @note Permite consultas por período e geração de rankings
 */
void MenuConsultas(BDados* bd) {
    system("clear || cls");
    int opcao;
    time_t data1, data2;
    do {
        printf("\n=== Menu Consultas ===\n");
        printf("1. Listagem ordenada por matrícula (período)\n");
        printf("2. Ranking de circulação (km percorridos)\n");
        printf("3. Ranking por marca (km percorridos)\n");
        printf("4. Listagem de infrações (velocidade >120km/h)\n");
        printf("5. Ranking de infrações\n");
        printf("6. Marca com maior velocidade média\n");
        printf("7. Dono com maior velocidade média\n");
        printf("8. Sobrenome mais comum\n");
        printf("9. Veículo mais rápido e seu condutor\n");
        printf("0. Voltar\n");
        
        opcao = lerInteiro("Escolha: ", 0, 9);
        
        switch(opcao) {
            case 1:
                data1 = lerDataHora();
                data2 = lerDataHora();
                listar_veiculos_periodo(bd->passagens, bd->veiculos, data1, data2);
                break; 
            case 2:
                data1 = lerDataHora();
                data2 = lerDataHora();
                listar_ranking_circulacao(bd->passagens, bd->veiculos, bd->distancias, data1, data2);
                break;
            case 3:
                data1 = lerDataHora();
                data2 = lerDataHora();
                listar_ranking_marca(bd->passagens, bd->veiculos, bd->distancias, data1, data2);
                break;
            case 4:
                data1 = lerDataHora();
                data2 = lerDataHora();
                listar_infracoes(bd->passagens, bd->veiculos, bd->distancias, data1, data2);
                break;
            case 5:
                data1 = lerDataHora();
                data2 = lerDataHora();
                listar_ranking_infracoes(bd->passagens, bd->veiculos, bd->distancias, data1, data2);
                break;
            case 6:
                listar_marca_maior_velocidade_media(bd->passagens, bd->veiculos, bd->distancias);
                break;
            case 7:
                listar_dono_maior_velocidade_media(bd->passagens, bd->veiculos, bd->distancias);  
                break;
            case 8:
                mostrar_subnome_mais_comum(bd->donos); 
                break;
            case 9:
                mostrar_veiculo_mais_rapido(bd->passagens, bd->veiculos, bd->distancias);
                break; 
            case 0: 
                break;
            default: 
                printf("Opcao invalida!\n");
        }
    } while (opcao != 0);
}

/**
 * @brief Menu de estatísticas do sistema
 * @param bd Ponteiro para a base de dados
 * @note Mostra principalmente informações de uso de memória
 */
void MenuEstatisticas(BDados* bd) {
    system("clear || cls");
    printf("\n=== Menu Estatisticas ===\n");
    CalcularMemoria(bd);

    int opcao;
    printf("Pressione 0 para voltar ao menu principal:");
    opcao = lerInteiro("Escolha: ", 0, 0);
}

/**
 * @brief Menu de exportação de dados
 * @param bd Ponteiro para a base de dados
 * @note Cria automaticamente os diretórios export/{csv,xml} se não existirem
 */
void MenuExportar(BDados* bd) {
    system("clear || cls");
    int opcao;
    
    // Criar diretórios (Windows)
    CreateDirectory("export", NULL);
    CreateDirectory("export\\csv", NULL);
    CreateDirectory("export\\xml", NULL);
    
    do {
        printf("\n=== Menu Exportar ===\n");
        printf("1. Exportar para CSV\n");
        printf("2. Exportar para XML\n");
        printf("0. Voltar\n");
        opcao = lerInteiro("Escolha: ", 0, 2);
        
        switch(opcao) {
            case 1: 
                exportDonoToCSV(bd->donos, "export\\csv\\donos.csv");
                exportVeiculoToCSV(bd->veiculos, "export\\csv\\veiculos.csv");
                exportSensorToCSV(bd->sensores, "export\\csv\\sensores.csv");
                exportPassagemToCSV(bd->passagens, "export\\csv\\passagens.csv");
                exportDistanciaToCSV(bd->distancias, "export\\csv\\distancias.csv");
                printf("Dados exportados para CSV com sucesso!\n");
                break;
            case 2: 
                exportToXML(bd->donos, bd->veiculos, bd->sensores, 
                          bd->passagens, bd->distancias, "export\\xml\\dados.xml");
                printf("Dados exportados para XML com sucesso!\n");
                break;
            case 0: 
                break;
            default: 
                printf("Opcao invalida!\n");
        }
    } while (opcao != 0);
}