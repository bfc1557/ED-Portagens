#include "menu.h"
#include <stdio.h>
#include <windows.h>
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
        printf("3. Listar ordenados por número contribuinte\n");
        printf("0. Voltar\n");
        opcao = lerInteiro("Escolha: ", 0, 3);
        
        switch(opcao) {
            case 1: registar_dono(bd->donos); break;
            case 2: listar_donos_alfabetico(bd->donos); break;
            case 3: 
                listar_donos_numContribuinte(bd->donos); 
                break;
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
<<<<<<< HEAD
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
=======
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

>>>>>>> origin/Duarte
}


void MenuConsultas(BDados* bd) {
<<<<<<< HEAD
    int opcao;
    do {
        printf("\n=== Menu Consultas ===\n");
        printf("1. Passagens por Período\n");
        printf("0. Voltar\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        
        switch(opcao) {
            case 1: break;
            case 0: break;
            default: printf("Opção inválida!\n");
=======
    system("clear || cls");
    int opcao;
    time_t data1, data2;
    do {
        printf("\n=== Menu Consultas ===\n");
        printf("1. Listagem ordenada por matrícula dos veículos que circularam autoestrada durante o período X. \n");
        printf("2. Ranking de circulacao. Listagem ordenada pelo total de quilometros que cada veiculo efectuou na auto-estrada durante determinado periodo\n");
        printf("3. Ranking por marca. Listagem ordenada pelo total de quilometros que cada marca efectuou na auto-estrada durante determinado periodo.\n");
        printf("4. Listagem de infrações. Contém a matrícula dos veículos que circularam com velocidade média superior a 120 km/h durante determinado período.\n");
        printf("5. Ranking de infrações. Listagem ordenada pelo número de infrações cometidas por cada veículo durante determinado período.\n");
        printf("6. Marca com maior velocidade média.\n");
        printf("7. Sobrenome mais comum.\n");
        printf("0. Voltar\n");
        opcao = lerInteiro("Escolha: ", 0, 7);
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
                mostrar_subnome_mais_comum(bd->donos);  
            case 0: break;
            default: printf("Opcao invalida!\n");
>>>>>>> origin/Duarte
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
    int opcao;
    
    // Criar diretórios (Windows)
    CreateDirectory("export", NULL);
    CreateDirectory("export\\csv", NULL);
    CreateDirectory("export\\xml", NULL);
    
    do {
        printf("\n=== Menu Exportar ===\n");
        printf("1. Exportar para CSV.\n");
        printf("2. Exportar para XML.\n");
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
                system("clear || cls");
            case 2: 
                exportToXML(bd->donos, bd->veiculos, bd->sensores, 
                          bd->passagens, bd->distancias, "export\\xml\\dados.xml");
                printf("Dados exportados para XML com sucesso!\n");
                break;
                system("clear || cls");
            case 0: break;
            default: printf("Opcao invalida!\n");
        }
    } while (opcao != 0);
}