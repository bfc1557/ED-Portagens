/**
 * @file main.c
 * @brief Programa principal de gestão de portagens
 * @details Implementa o menu principal e fluxo de operações do sistema
 */

#include "menu.h"
#include "../libs/bdados.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Função principal do programa
 * @return 0 se o programa terminar com sucesso
 * 
 * @note Fluxo principal do programa:
 * 1. Inicializa a estrutura de dados
 * 2. Carrega os dados dos ficheiros
 * 3. Apresenta o menu principal em loop
 * 4. Executa a opção selecionada
 * 5. Antes de sair, guarda os dados nos ficheiros
 * 
 * @warning A diretoria "../data" deve existir e conter os ficheiros necessários
 */
int main() {
    // Inicializa a estrutura principal com o nome do sistema
    BDados *BD = InicializaPrograma("Dados Portagens");
    
    // Carrega os dados da diretoria "../data"
    CarregarDados(BD, "../data");
    
    int opcao;
    do {
        // Mostra o menu principal e obtém a opção do utilizador
        opcao = MenuPrincipal();
        
        switch(opcao) {
            case 1: 
                MenuDonos(BD);  // Gestão de donos (funcional)
                break;
            case 2: 
                MenuVeiculos(BD);  // Gestão de veículos (funcional)
                break;
            case 3: 
                MenuPassagens(BD);  // Gestão de passagens
                break;
            case 4: 
                MenuConsultas(BD);  // Menu de consultas
                break;
            case 5: 
                MenuEstatisticas(BD);  // Menu de estatísticas
                break;
            case 6: 
                MenuExportar(BD);  // Menu de exportação de dados
                break;
            case 0: 
                printf("A sair...\n");
                // Guarda os dados atualizados nos ficheiros
                salvar_donos(BD->donos, "../data/donos.txt");
                salvar_veiculos(BD->veiculos, "../data/carros.txt");
                salvar_passagens(BD->passagens, "../data/passagem.txt");
                break;
            default: 
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);  // Repete até o utilizador selecionar sair
    
    // Liberta toda a memória alocada
    FinalizarPrograma(BD);
    
    return 0;
}