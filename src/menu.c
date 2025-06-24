#include "menu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int MenuPrincipal() {
    printf("\n=== Menu Principal ===\n");
    printf("1. Donos\n");
    printf("2. Veículos\n");
    printf("3. Passagens\n");
    printf("4. Consultas\n");
    printf("5. Estatísticas\n");
    printf("6. Exportar\n");
    printf("0. Sair\n");
    
    int opcao;
    printf("Escolha: ");
    scanf("%d", &opcao);
    return opcao;
}

void MenuDonos(BDados* bd) {
    int opcao;
    do {
        printf("\n=== Menu Donos ===\n");
        printf("1. Registar dono\n");
        printf("2. Listar ordenados alfabeticamente\n");
        printf("3. Listar ordenados por número contribuinte\n");
        printf("0. Voltar\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        
        switch(opcao) {
            case 1: registar_dono(bd->donos); break;
            case 2: listar_donos_alfabetico(bd->donos); break;
            case 3: listar_donos_numContribuinte(bd->donos); break;
            case 0: break;
            default: printf("Opção inválida!\n");
        }
    } while (opcao != 0);
}

void MenuVeiculos(BDados* bd) {
    int opcao;
    do {
        printf("\n=== Menu Veículos ===\n");
        printf("1. Registar veículo\n");
        printf("2. Listar por matrícula\n");
        printf("3. Listar por marca\n");
        printf("4. Listar por modelo\n");
        printf("0. Voltar\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        
        switch(opcao) {
            case 1: registar_veiculo(bd->veiculos, bd->donos); break;
            case 2: listar_veiculos_ordenados(bd->veiculos, "matricula"); break;
            case 3: listar_veiculos_ordenados(bd->veiculos, "marca"); break;
            case 4: listar_veiculos_ordenados(bd->veiculos, "modelo"); break;
            case 0: break;
            default: printf("Opção inválida!\n");
        }
    } while (opcao != 0);
}

void MenuPassagens(BDados* bd) {
    int opcao;
    do {
        printf("\n=== Menu Passagens ===\n");
        printf("1. Registar passagem\n");
        printf("0. Voltar\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        
        switch(opcao) {
            case 1: registar_passagem(&bd->passagens, bd->veiculos); break;
            case 0: break;
            default: printf("Opção inválida!\n");
        }
    } while (opcao != 0);
}

void MenuConsultas(BDados* bd) {
    int opcao;
    time_t data1, data2;
    char a;
    do {
        printf("\n=== Menu Consultas ===\n");
        printf("1. Listagem ordenada por matrícula dos veículos que circularam autoestrada durante o período X. \n");
        printf("2. Ranking de circulacao. Listagem ordenada pelo total de quilometros que cada veiculo efectuou na auto-estrada durante determinado periodo\n");
        printf("3. Listar ordenados por número contribuinte\n");
        printf("0. Voltar\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        
        switch(opcao) {
            case 1: // A funcionar
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
            default: printf("Opção inválida!\n");
        }
    } while (opcao != 0);
    printf("Funcionalidade a implementar na próxima fase\n");
}

void MenuEstatisticas(BDados* bd) {
    printf("\n=== Menu Estatísticas ===\n");
    CalcularMemoria(bd);
}

void MenuExportar(BDados* bd) {
    printf("\n=== Menu Exportar ===\n");
    printf("Funcionalidade a implementar na próxima fase\n");
}