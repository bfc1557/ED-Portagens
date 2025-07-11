/**
 * @file export.h
 * @brief Interface para exportação de dados do sistema para formato XML
 * @details Contém a declaração da função principal de exportação de dados
 */

#ifndef EXPORT_H
#define EXPORT_H

#include "../libs/bdados.h"
#include "../structs/distancia/distancia.h"
#include "../structs/dono/dono.h"
#include "../structs/passagem/passagem.h"
#include "../structs/sensor/sensor.h"
#include "../structs/veiculo/veiculo.h"

/**
 * @brief Exporta os dados do sistema para um arquivo XML formatado
 * 
 * @param donoTable Tabela hash contendo todos os donos registrados
 * @param veiculoTable Tabela hash contendo todos os veículos registrados
 * @param sensorList Lista ligada de sensores do sistema
 * @param passagemList Lista ligada de registros de passagem
 * @param distanciaList Lista ligada de distâncias entre sensores
 * @param filename Nome do arquivo XML de destino (caminho completo ou relativo)
 * 
 * @note A função cria um arquivo XML bem formatado com todos os dados do sistema
 * @warning Se o arquivo já existir, será sobrescrito sem aviso
 * 
 * @see Para o formato exato do XML gerado, consulte a implementação em export.c
 * 
 * @example 
 * // Exemplo de uso básico:
 * exportToXML(donos, veiculos, sensores, passagens, distancias, "backup.xml");
 * 
 * @example
 * // Exemplo com caminho completo:
 * exportToXML(donos, veiculos, sensores, passagens, distancias, "/backups/full_export.xml");
 */
void exportToXML(DonoHashTable *donoTable, VeiculoHashTable *veiculoTable,
                SensorList sensorList, PassagemList passagemList,
                DistanciaList distanciaList, const char *filename);

#endif