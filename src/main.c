#include "menu.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    BDados *BD = InicializaPrograma("Dados Portagens");
    CarregarDados(BD, "../data");
    
    int opcao;
    do {
        opcao = MenuPrincipal();
        switch(opcao) {
            case 1: MenuDonos(BD); break; // Tudo a funcionar
            case 2: MenuVeiculos(BD); break; // Tudo a funcionar
            case 3: MenuPassagens(BD); break;
            case 4: MenuConsultas(BD); break;
            case 5: MenuEstatisticas(BD); break;
            case 6: MenuExportar(BD); break;
            case 0: printf("A sair...\n"); break;
            default: printf("Opção inválida!\n");
        }
    } while (opcao != 0);
    
    FinalizarPrograma(BD);
    return 0;
}