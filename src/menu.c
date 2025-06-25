#include "menu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

void MenuDonos(BDados* bd) {
    system("clear || cls");
    int opcao;
    do {
        printf("\n=== Menu Donos ===\n");
        printf("1. Registar dono\n");
        printf("2. Listar ordenados alfabeticamente\n");
        printf("0. Voltar\n");
        opcao = lerInteiro("Escolha: ", 0, 2);
        
        switch(opcao) {
            case 1: registar_dono(bd->donos); break;
            case 2: listar_donos_alfabetico(bd->donos); break;
            case 0: break;
            default: printf("Opcao invalida!\n");
        }
    } while (opcao != 0);
}

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
            case 1: registar_veiculo(bd->veiculos, bd->donos); break;
            case 2: listar_veiculos_ordenados(bd->veiculos, "matricula"); break;
            case 3: listar_veiculos_ordenados(bd->veiculos, "marca"); break;
            case 4: listar_veiculos_ordenados(bd->veiculos, "modelo"); break;
            case 0: break;
            default: printf("Opcao invalida!\n");
        }
    } while (opcao != 0);
}

void MenuPassagens(BDados* bd) {
    system("clear || cls");
    int opcao;
    do {
        printf("\n=== Menu Passagens ===\n");
        printf("1. Registar Passagem\n");
        printf("0. Voltar\n");
        opcao = lerInteiro("Escolha: ", 0, 1);
        
        switch(opcao) {
            case 1: registar_passagem_interativo(&bd->passagens); break;
            case 0: break;
            default: printf("Opcao invalida!\n");
        }
    } while (opcao != 0);

}

void MenuConsultas(BDados* bd) {
    system("clear || cls");
    int opcao;
    time_t data1, data2;
    do {
        printf("\n=== Menu Consultas ===\n");
        printf("1. Listagem ordenada por matrícula dos veículos que circularam autoestrada durante o período X. \n");
        printf("2. Ranking de circulacao. Listagem ordenada pelo total de quilometros que cada veiculo efectuou na auto-estrada durante determinado periodo\n");
        printf("3. Listar ordenados por número contribuinte\n");
        printf("0. Voltar\n");
        opcao = lerInteiro("Escolha: ", 0, 3);
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
            case 3: listar_donos_numContribuinte(bd->donos); break;
            case 0: break;
            default: printf("Opcao invalida!\n");
        }
    } while (opcao != 0);
}

void MenuEstatisticas(BDados* bd) {
    system("clear || cls");
    printf("\n=== Menu Estatisticas ===\n");
    CalcularMemoria(bd);

    int opcao;
    printf("Pressione 0 para voltar ao menu principal:");
    opcao = lerInteiro("Escolha: ", 0, 0);
}

void MenuExportar(BDados* bd) {
    system("clear || cls");
    printf("\n=== Menu Exportar ===\n");
    printf("Funcionalidade a implementar na próxima fase\n");
}