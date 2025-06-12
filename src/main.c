#include "menu.h"
#include "bdados.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    BDados *BD = InicializaPrograma("Dados Portagens");
    CarregarDados(BD, "../data");
    system("cls");
    
    int opcao;
    do {
        opcao = MenuPrincipal();
        switch(opcao) {
            case 1: MenuDonos(BD); break; // Tudo a funcionar
            case 2: MenuVeiculos(BD); break; // Tudo a funcionar
            case 3: MenuPassagens(BD); break;
            case 4: MenuConsultas(BD); break;
            case 5: MenuEstatisticas(BD); break; // Tudo a funcionar
            case 6: MenuExportar(BD); break;
            case 0: 
            printf("A sair...\n");
            salvar_donos(BD->donos, "../data/donos.txt");
            salvar_veiculos(BD->veiculos, "../data/carros.txt");
            break;
            default: printf("Opção inválida!\n");
        }
    } while (opcao != 0);
    
    FinalizarPrograma(BD);
    return 0;
}