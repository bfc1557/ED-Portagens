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
        printf("3. Listar ordenados por numero contribuinte\n");
        printf("0. Voltar\n");
        opcao = lerInteiro("Escolha: ", 0, 3);
        
        switch(opcao) {
            case 1: registar_dono(bd->donos); break;
            case 2: listar_donos_alfabetico(bd->donos); break;
            case 3: listar_donos_numContribuinte(bd->donos); break;
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
    printf("\n=== Menu Consultas ===\n");
    printf("Funcionalidade a implementar na próxima fase\n");
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