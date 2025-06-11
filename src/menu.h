#ifndef MENU_H
#define MENU_H

#include "../libs/bdados.h"

int MenuPrincipal();
void MenuDonos(BDados* bd);
void MenuVeiculos(BDados* bd);
void MenuPassagens(BDados* bd);
void MenuConsultas(BDados* bd);
void MenuEstatisticas(BDados* bd);
void MenuExportar(BDados* bd);

#endif