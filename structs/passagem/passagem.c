#include "passagem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

PassagemList criar_lista_passagens() {
    return NULL;
}

void liberar_lista_passagens(PassagemList list) {
    PassagemNode* current = list;
    while (current) {
        PassagemNode* temp = current;
        current = current->next;
        free(temp);
    }
}

void inserir_passagem(PassagemList* list, Passagem passagem) {
    PassagemNode* newNode = (PassagemNode*)malloc(sizeof(PassagemNode));
    newNode->passagem = passagem;
    newNode->next = *list;
    *list = newNode;
}

void carregar_passagens(PassagemList* list, const char* filename, int batch_size) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir arquivo de passagens");
        return;
    }

    char line[256];
    int count = 0;
    while (fgets(line, sizeof(line), file)) {
        Passagem passagem;
        int day, month, year, hour, min, sec, ms;
        
        if (sscanf(line, "%d\t%d\t%d-%d-%d %d:%d:%d.%d\t%d",
                  &passagem.idSensor, &passagem.codVeiculo,
                  &day, &month, &year, &hour, &min, &sec, &ms,
                  &passagem.tipoRegisto) == 10) {
            
            struct tm tm = {0};
            tm.tm_year = year - 1900;
            tm.tm_mon = month - 1;
            tm.tm_mday = day;
            tm.tm_hour = hour;
            tm.tm_min = min;
            tm.tm_sec = sec;
            passagem.data = mktime(&tm);
            
            inserir_passagem(list, passagem);
            
            // Controle de lotes para arquivos grandes
            if (++count % batch_size == 0) {
                printf("Carregadas %d passagens...\n", count);
            }
        }
    }
    fclose(file);
    printf("Total de passagens carregadas: %d\n", count);
}

int calcular_memoria_passagens(PassagemList list) {
    int total = 0;
    PassagemNode* current = list;
    while (current) {
        total += sizeof(PassagemNode);
        current = current->next;
    }
    return total;
}

void listar_passagens_periodo(PassagemList list, time_t inicio, time_t fim) {
    PassagemNode* current = list;
    while (current) {
        if (current->passagem.data >= inicio && current->passagem.data <= fim) {
            char buffer[80];
            strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&current->passagem.data));
            printf("Sensor: %d, Veículo: %d, Data: %s, Tipo: %s\n",
                  current->passagem.idSensor, current->passagem.codVeiculo,
                  buffer, current->passagem.tipoRegisto ? "Saída" : "Entrada");
        }
        current = current->next;
    }
}

void listar_veiculos_periodo(PassagemList passagens, VeiculoHashTable* veiculos, time_t inicio, time_t fim) {
    if (!passagens || !veiculos) return;

    // Primeiro, coletar todos os veículos únicos no período
    Veiculo** veiculos_periodo = NULL;
    int count = 0;
    int capacity = 0;

    PassagemNode* current = passagens;
    while (current) {
        if (current->passagem.data >= inicio && current->passagem.data <= fim) {
            Veiculo* veiculo = buscar_veiculo_codigo(veiculos, current->passagem.codVeiculo);
            
            if (veiculo) {
                // Verificar se já está na lista
                int encontrado = 0;
                for (int i = 0; i < count; i++) {
                    if (veiculos_periodo[i] == veiculo) {
                        encontrado = 1;
                        break;
                    }
                }
                
                if (!encontrado) {
                    if (count >= capacity) {
                        capacity = capacity == 0 ? 16 : capacity * 2;
                        Veiculo** temp = (Veiculo**)realloc(veiculos_periodo, capacity * sizeof(Veiculo*));
                        if (!temp) {
                            perror("Erro ao realocar memória");
                            free(veiculos_periodo);
                            return;
                        }
                        veiculos_periodo = temp;
                    }
                    veiculos_periodo[count++] = veiculo;
                }
            }
        }
        current = current->next;
    }

    // Ordenar por matrícula
    merge_sort((void**)veiculos_periodo, 0, count - 1, comparar_veiculos_matricula);

    // Imprimir resultados
    printf("\n=== Veículos no período selecionado ===\n");
    printf("%-10s %-15s %-15s %-4s %s\n", "Matrícula", "Marca", "Modelo", "Ano", "Dono");
    
    for (int i = 0; i < count; i++) {
        printf("%-10s %-15s %-15s %-4d %s\n", 
               veiculos_periodo[i]->matricula,
               veiculos_periodo[i]->marca,
               veiculos_periodo[i]->modelo,
               veiculos_periodo[i]->ano,
               veiculos_periodo[i]->dono ? veiculos_periodo[i]->dono->nome : "N/A");
    }

    free(veiculos_periodo);
}

void registar_passagem(PassagemList* list, VeiculoHashTable* veiculos){
    Passagem nova_passagem;
    printf("\n=== Registar Nova Passagem ===\n");
    
    printf("ID do Sensor: ");
    scanf("%d", &nova_passagem.idSensor);
    
    printf("Código do Veículo: ");
    scanf("%d", &nova_passagem.codVeiculo);
    
    printf("Tipo de Registo (0=Entrada, 1=Saída): ");
    scanf("%d", &nova_passagem.tipoRegisto);

    // Obter data e hora atual
    nova_passagem.data = time(NULL);
    if (nova_passagem.data == (time_t)-1) {
        perror("Erro ao obter data e hora atual");
        return;
    }
    
    // Verificar se o veículo existe
    if (!buscar_veiculo_codigo(veiculos, nova_passagem.codVeiculo)) {
        printf("Erro: Veículo com código %d não encontrado!\n", nova_passagem.codVeiculo);
        return;
    }
    
    inserir_passagem(list, nova_passagem);
    printf("Passagem registada com sucesso!\n");
}

void salvar_passagens(PassagemList list, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Erro ao abrir arquivo de passagens para escrita");
        return;
    }

    PassagemNode* current = list;
    while (current) {
        struct tm* tm_info = localtime(&current->passagem.data);
        char buffer[80];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_info);
        
        fprintf(file, "%d\t%d\t%s\t%d\n",
                current->passagem.idSensor,
                current->passagem.codVeiculo,
                buffer,
                current->passagem.tipoRegisto);
        
        current = current->next;
    }

    fclose(file);
    printf("Passagens salvas com sucesso em %s\n", filename);
}