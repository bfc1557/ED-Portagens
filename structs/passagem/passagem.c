#include "passagem.h"
#include "../veiculo/veiculo.h"
#include "../distancia/distancia.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    Veiculo* veiculo;
    float total_km;
} RankingItem;

PassagemList criar_lista_passagens() {
    return NULL;
}

void libertar_lista_passagens(PassagemList list) {
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

    void** veiculos_periodo = NULL;
    int count = 0;
    int capacity = 0;

    PassagemNode* current = passagens;
    while (current) {
        if (current->passagem.data >= inicio && current->passagem.data <= fim) {
            Veiculo* veiculo = buscar_veiculo_codigo(veiculos, current->passagem.codVeiculo);
            
            if (veiculo) {
                // Verifica se já está no array
                int encontrado = 0;
                for (int i = 0; i < count; i++) {
                    if (veiculos_periodo[i] == (void*)veiculo) {
                        encontrado = 1;
                        break;
                    }
                }

                if (!encontrado) {
                    if (count >= capacity) {
                        capacity = (capacity == 0) ? 16 : capacity * 2;
                        void** temp = realloc(veiculos_periodo, capacity * sizeof(void*));
                        if (!temp) {
                            perror("Erro ao realocar memória");
                            free(veiculos_periodo);
                            return;
                        }
                        veiculos_periodo = temp;
                    }
                    veiculos_periodo[count++] = (void*)veiculo;
                }
            }
        }
        current = current->next;
    }

    if (count == 0) {
        printf("Nenhum veículo encontrado no período selecionado.\n");
        return;
    }

    // Ordenar os ponteiros genéricos por matrícula
    merge_sort(veiculos_periodo, 0, count - 1, comparar_veiculos_matricula);

    // Imprimir cabeçalho
    printf("\n=== Veículos no período selecionado ===\n");
    printf("%-10s %-15s %-15s %-4s %s\n", "Matrícula", "Marca", "Modelo", "Ano", "Dono");

    // Paginar resultados, usando função de impressão que faz cast interno
    paginacao(veiculos_periodo, count, 30, mostrarCarro);

    free(veiculos_periodo);
}

int comparar_passagem_por_data(const void* a, const void* b) {
    const Passagem* p1 = *(const Passagem**)a;
    const Passagem* p2 = *(const Passagem**)b;

    if (p1->data < p2->data) return -1;
    else if (p1->data > p2->data) return 1;
    else return 0;
}

int comparar_ranking_item_por_km(const void* a, const void* b) {
    const RankingItem* r1 = (const RankingItem*)a;
    const RankingItem* r2 = (const RankingItem*)b;

    if (r1->total_km < r2->total_km) return 1;
    else if (r1->total_km > r2->total_km) return -1;
    else return 0;
}

void mostrarRankingKmPVeiculo(void *dado)
{
    RankingItem *ptr;
    ptr = (RankingItem*) dado;
    Veiculo* v = ptr->veiculo;
        printf("%-10s %-15s %-15s %-4d %-10.2f %s\n",
               v->matricula, v->marca, v->modelo, v->ano,
               ptr->total_km,
               v->dono ? v->dono->nome : "N/A");
}

void listar_ranking_circulacao(PassagemList passagens, VeiculoHashTable* veiculos, DistanciaList distancias, time_t inicio, time_t fim) {
    if (!passagens || !veiculos) return;

    RankingItem* ranking = NULL;
    int count = 0, capacity = 0;

    // Etapa 1: Agrupar veículos únicos no período
    for (PassagemNode* current = passagens; current; current = current->next) {
        if (current->passagem.data < inicio || current->passagem.data > fim) continue;

        Veiculo* veiculo = buscar_veiculo_codigo(veiculos, current->passagem.codVeiculo);
        if (!veiculo) continue;

        int i, encontrado = 0;
        for (i = 0; i < count; i++) {
            if (ranking[i].veiculo == veiculo) {
                encontrado = 1;
                break;
            }
        }

        if (!encontrado) {
            if (count >= capacity) {
                capacity = (capacity == 0) ? 16 : capacity * 2;
                RankingItem* temp = realloc(ranking, capacity * sizeof(RankingItem));
                if (!temp) {
                    perror("Erro ao realocar memória");
                    free(ranking);
                    return;
                }
                ranking = temp;
            }
            ranking[count].veiculo = veiculo;
            ranking[count].total_km = 0.0f;
            count++;
        }
    }

    // Etapa 2: Para cada veículo, calcular total de km
    for (int i = 0; i < count; i++) {
        Passagem** p_array = NULL;
        int p_count = 0, p_cap = 0;

        // Recolher passagens desse veículo
        for (PassagemNode* node = passagens; node; node = node->next) {
            if (node->passagem.codVeiculo == ranking[i].veiculo->codVeiculo &&
                node->passagem.data >= inicio && node->passagem.data <= fim) {

                if (p_count >= p_cap) {
                    p_cap = (p_cap == 0) ? 8 : p_cap * 2;
                    Passagem** temp = realloc(p_array, p_cap * sizeof(Passagem*));
                    if (!temp) break;
                    p_array = temp;
                }
                p_array[p_count++] = &node->passagem;
            }
        }

        // Ordenar passagens por data
        qsort(p_array, p_count, sizeof(Passagem*), comparar_passagem_por_data);

        // Calcular distâncias entre pares entrada/saída
        for (int j = 0; j < p_count - 1; j++) {
            if (p_array[j]->tipoRegisto == 0 && p_array[j + 1]->tipoRegisto == 1) {
                float d = obter_distancia_entre_sensores(distancias, p_array[j]->idSensor, p_array[j + 1]->idSensor);
                ranking[i].total_km += d;
                j++; // saltar o par usado
            }
        }

        free(p_array);
    }

    // Etapa 3: Ordenar por total_km decrescente
    qsort(ranking, count, sizeof(RankingItem), comparar_ranking_item_por_km);

    // Etapa 4: Criar array de ponteiros para passar à paginacao
    void** ranking_ptrs = malloc(count * sizeof(void*));
    if (!ranking_ptrs) {
        perror("Erro ao alocar memória para ponteiros do ranking");
        free(ranking);
        return;
    }
    for (int i = 0; i < count; i++) {
        ranking_ptrs[i] = &ranking[i];
    }

    // Etapa 5: Mostrar com paginacao
    printf("\n=== Ranking de circulação no período ===\n");
    printf("%-10s %-15s %-15s %-4s %-10s %s\n", "Matrícula", "Marca", "Modelo", "Ano", "KM", "Dono");
    paginacao(ranking_ptrs, count, 30, mostrarRankingKmPVeiculo);

    free(ranking_ptrs);
    free(ranking);
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
