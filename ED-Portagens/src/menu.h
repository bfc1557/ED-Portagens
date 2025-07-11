#ifndef MENU_H
#define MENU_H

#include "../libs/bdados.h"
#include "../libs/export.h"

/**
 * @brief Mostra o menu principal da aplicação.
 * 
 * @return Código da opção escolhida pelo utilizador.
 */
int MenuPrincipal();

/**
 * @brief Mostra o menu de gestão de Donos.
 * 
 * @param bd Apontador para a estrutura de dados principal.
 */
void MenuDonos(BDados* bd);

/**
 * @brief Mostra o menu de gestão de Veículos.
 * 
 * @param bd Apontador para a estrutura de dados principal.
 */
void MenuVeiculos(BDados* bd);

/**
 * @brief Mostra o menu de gestão de Passagens.
 * 
 * @param bd Apontador para a estrutura de dados principal.
 */
void MenuPassagens(BDados* bd);

/**
 * @brief Mostra o menu de Consultas.
 * 
 * @param bd Apontador para a estrutura de dados principal.
 */
void MenuConsultas(BDados* bd);

/**
 * @brief Mostra o menu de Estatísticas.
 * 
 * @param bd Apontador para a estrutura de dados principal.
 */
void MenuEstatisticas(BDados* bd);

/**
 * @brief Mostra o menu de Exportação de dados.
 * 
 * @param bd Apontador para a estrutura de dados principal.
 */
void MenuExportar(BDados* bd);

#endif
