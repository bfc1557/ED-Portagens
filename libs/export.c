#include <stdio.h>
#include <time.h>
#include "export.h"

void exportToXML(DonoHashTable *donoTable, VeiculoHashTable *veiculoTable,
                SensorList sensorList, PassagemList passagemList,
                DistanciaList distanciaList, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Erro ao abrir arquivo XML");
        return;
    }

    fprintf(file, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(file, "<dados>\n");
    
    // Donos (mantido igual)
    fprintf(file, "  <donos>\n");
    for (int i = 0; i < donoTable->size; i++) {
        DonoNode *current = donoTable->buckets[i];
        while (current != NULL) {
            fprintf(file, "    <dono>\n");
            fprintf(file, "      <numContribuinte>%d</numContribuinte>\n", current->dono.numContribuinte);
            fprintf(file, "      <nome><![CDATA[%s]]></nome>\n", current->dono.nome);
            fprintf(file, "      <codPostal>%s</codPostal>\n", current->dono.codPostal);
            fprintf(file, "    </dono>\n");
            current = current->next;
        }
    }
    fprintf(file, "  </donos>\n");
    
    // Veículos (agora é o segundo parâmetro)
    fprintf(file, "  <veiculos>\n");
    for (int i = 0; i < veiculoTable->size; i++) {
        VeiculoNode *current = veiculoTable->buckets[i];
        while (current != NULL) {
            fprintf(file, "    <veiculo>\n");
            fprintf(file, "      <matricula>%s</matricula>\n", current->veiculo.matricula);
            fprintf(file, "      <marca><![CDATA[%s]]></marca>\n", current->veiculo.marca);
            fprintf(file, "      <modelo><![CDATA[%s]]></modelo>\n", current->veiculo.modelo);
            fprintf(file, "      <ano>%d</ano>\n", current->veiculo.ano);
            fprintf(file, "      <codVeiculo>%d</codVeiculo>\n", current->veiculo.codVeiculo);
            if (current->veiculo.dono) {
                fprintf(file, "      <dono>%d</dono>\n", current->veiculo.dono->numContribuinte);
            }
            fprintf(file, "    </veiculo>\n");
            current = current->next;
        }
    }
    fprintf(file, "  </veiculos>\n");
    
    // Sensores (terceiro parâmetro)
    fprintf(file, "  <sensores>\n");
    SensorNode *sensorCurrent = sensorList;
    while (sensorCurrent != NULL) {
        fprintf(file, "    <sensor>\n");
        fprintf(file, "      <codSensor>%d</codSensor>\n", sensorCurrent->sensor.codSensor);
        fprintf(file, "      <designacao><![CDATA[%s]]></designacao>\n", sensorCurrent->sensor.designacao);
        fprintf(file, "      <latitude>%s</latitude>\n", sensorCurrent->sensor.latitude);
        fprintf(file, "      <longitude>%s</longitude>\n", sensorCurrent->sensor.longitude);
        fprintf(file, "    </sensor>\n");
        sensorCurrent = sensorCurrent->next;
    }
    fprintf(file, "  </sensores>\n");
    
    // Passagens (quarto parâmetro)
    fprintf(file, "  <passagens>\n");
    PassagemNode *passagemCurrent = passagemList;
    while (passagemCurrent != NULL) {
        char dateStr[20];
        strftime(dateStr, sizeof(dateStr), "%Y-%m-%d %H:%M:%S", localtime(&passagemCurrent->passagem.data));
        
        fprintf(file, "    <passagem>\n");
        fprintf(file, "      <idSensor>%d</idSensor>\n", passagemCurrent->passagem.idSensor);
        fprintf(file, "      <codVeiculo>%d</codVeiculo>\n", passagemCurrent->passagem.codVeiculo);
        fprintf(file, "      <data>%s</data>\n", dateStr);
        fprintf(file, "      <milissegundos>%d</milissegundos>\n", passagemCurrent->passagem.milissegundos);
        fprintf(file, "      <tipoRegisto>%s</tipoRegisto>\n", 
                passagemCurrent->passagem.tipoRegisto == 0 ? "entrada" : "saida");
        fprintf(file, "    </passagem>\n");
        passagemCurrent = passagemCurrent->next;
    }
    fprintf(file, "  </passagens>\n");
    
    // Distâncias (quinto parâmetro)
    fprintf(file, "  <distancias>\n");
    DistanciaNode *distanciaCurrent = distanciaList;
    while (distanciaCurrent != NULL) {
        fprintf(file, "    <distancia>\n");
        fprintf(file, "      <codSensor1>%d</codSensor1>\n", distanciaCurrent->distancia.codSensor1);
        fprintf(file, "      <codSensor2>%d</codSensor2>\n", distanciaCurrent->distancia.codSensor2);
        fprintf(file, "      <valor>%.2f</valor>\n", distanciaCurrent->distancia.distancia);
        fprintf(file, "    </distancia>\n");
        distanciaCurrent = distanciaCurrent->next;
    }
    fprintf(file, "  </distancias>\n");
    
    fprintf(file, "</dados>\n");
    fclose(file);
}