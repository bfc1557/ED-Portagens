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
    printf("\n=== Menu Passagens ===\n");
    printf("Funcionalidade a implementar na próxima fase\n");
}

void MenuConsultas(BDados* bd) {
    printf("\n=== Menu Consultas ===\n");
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