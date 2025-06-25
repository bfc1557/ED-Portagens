#include "passagem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

typedef struct {
    Veiculo* veiculo;
    float total_km;
} RankingItem;

PassagemList criar_lista_passagens(void) {
    return NULL;
}

void liberar_lista_passagens(PassagemList list) {
    while (list) {
        PassagemNode *tmp = list;
        list = list->next;
        free(tmp);
    }
}

void inserir_passagem(PassagemList *list, Passagem p) {
    PassagemNode *n = malloc(sizeof *n);
    if (!n) {
        perror("malloc inserir_passagem");
        return;
    }
    n->passagem = p;
    n->next     = *list;
    *list       = n;
}

static int parse_linha_passagem(const char *linha, Passagem *p) {
    int d,m,y,hh,mm,ss,ms;
    int lidos = sscanf(linha,
                       "%d\t%d\t%d-%d-%d %d:%d:%d.%d\t%d",
                       &p->idSensor,
                       &p->codVeiculo,
                       &d,&m,&y,
                       &hh,&mm,&ss,&ms,
                       &p->tipoRegisto);

    if (lidos != 10)               return -1; 
    if (p->idSensor <= 0 || p->codVeiculo <= 0) return -2; 
    if (p->tipoRegisto != 0 && p->tipoRegisto != 1) return -3; 
    if (ms < 0 || ms > 999)        return -5;  // Novo erro para ms inválidos

    struct tm t = {0};
    t.tm_mday = d;
    t.tm_mon  = m-1;
    t.tm_year = y-1900;
    t.tm_hour = hh;
    t.tm_min  = mm;
    t.tm_sec  = ss;

    time_t ts = mktime(&t);
    if (ts == (time_t)-1)          return -4; 

    p->data = ts;
    p->milissegundos = ms;  // Armazena os milissegundos
    return 0;
}

void carregar_passagens(PassagemList *list, const char *ficheiro, int batch) {
    FILE *f = fopen(ficheiro, "r");
    if (!f) {
        perror("carregar_passagens fopen");
        return;
    }

    char linha[256];
    long ok=0, err_fmt=0, err_neg=0, err_tipo=0, err_data=0, err_ms=0;

    while (fgets(linha, sizeof linha, f)) {
        Passagem p;
        int rc = parse_linha_passagem(linha, &p);
        if (rc == 0) {
            inserir_passagem(list, p);
            if (++ok % batch == 0)
                printf("Carregadas %ld passagens...\n", ok);
        } else {
            if (rc == -1) err_fmt++;
            else if (rc == -2) err_neg++;
            else if (rc == -3) err_tipo++;
            else if (rc == -4) err_data++;
            else if (rc == -5) err_ms++;
        }
    }

    fclose(f);

    printf("\n-- Resumo carregar_passagens --\n");
    printf("Sucesso             : %ld\n", ok);
    printf("Erros formato       : %ld\n", err_fmt);
    printf("Erros negativos     : %ld\n", err_neg);
    printf("Erros tipo registo  : %ld\n", err_tipo);
    printf("Erros data impossível: %ld\n", err_data);
    printf("Erros milissegundos : %ld\n", err_ms);
    printf("-------------------------------\n\n");
}

int calcular_memoria_passagens(PassagemList list) {
    int bytes = 0;
    for (PassagemNode *cur = list; cur; cur = cur->next)
        bytes += sizeof(PassagemNode);
    return bytes;
}

void listar_passagens_periodo(PassagemList list, time_t ini, time_t fim) {
    for (PassagemNode *cur = list; cur; cur = cur->next) {
        if (cur->passagem.data < ini || cur->passagem.data > fim) continue;
        char buf[64];
        strftime(buf,sizeof buf,"%d-%m-%Y %H:%M:%S",localtime(&cur->passagem.data));
        printf("Sensor:%3d  Veículo:%5d  %s.%03d  %s\n",
               cur->passagem.idSensor,
               cur->passagem.codVeiculo,
               buf,
               cur->passagem.milissegundos,
               cur->passagem.tipoRegisto?"Saída":"Entrada");
    }
}

int registar_passagem(PassagemList *list,
                     int idSensor,
                     int codVeiculo,
                     struct tm data_tm,
                     int milissegundos,
                     int tipoRegisto) {

    if (idSensor<=0 || codVeiculo<=0 || (tipoRegisto!=0 && tipoRegisto!=1))
        return -3;
    
    if (milissegundos < 0 || milissegundos > 999)
        return -5;

    time_t nova = mktime(&data_tm);
    if (nova == (time_t)-1) return -1;

    time_t ultima = 0;
    for (PassagemNode *cur = *list; cur; cur = cur->next) {
        if (cur->passagem.codVeiculo == codVeiculo && cur->passagem.data > ultima)
            ultima = cur->passagem.data;
    }

    if (ultima && difftime(nova, ultima) < 0)
        return -2;

    Passagem p = { 
        .idSensor = idSensor,
        .codVeiculo = codVeiculo,
        .data = nova,
        .milissegundos = milissegundos,
        .tipoRegisto = tipoRegisto 
    };
    inserir_passagem(list, p);
    return 0;
}

static void limpar_stdin(void) {
    int ch; while ((ch=getchar())!='\n' && ch!=EOF);
}

void registar_passagem_interativo(PassagemList *list) {
    int idSensor, codVeiculo, tipo, ms;
    char str[128];

    puts("\n=== Registar Nova Passagem ===");

    printf("ID do sensor: ");
    if (scanf("%d", &idSensor)!=1 || idSensor<=0) {
        fprintf(stderr,"Erro: ID do sensor inválido.\n");
        limpar_stdin(); return;
    }

    printf("Código do veículo: ");
    if (scanf("%d", &codVeiculo)!=1 || codVeiculo<=0) {
        fprintf(stderr,"Erro: código de veículo inválido.\n");
        limpar_stdin(); return;
    }
    limpar_stdin();

    printf("Data e hora (dd-mm-aaaa HH:MM:SS.mmm): ");
    if (!fgets(str,sizeof str,stdin)) { perror("fgets"); return; }

    int d,m,y,hh,mm,ss;
    if (sscanf(str,"%d-%d-%d %d:%d:%d.%d",&d,&m,&y,&hh,&mm,&ss,&ms)!=7) {
        fprintf(stderr,"Formato de data/hora inválido.\n");
        return;
    }

    printf("Tipo de registo (0=entrada, 1=saída): ");
    if (scanf("%d", &tipo)!=1 || (tipo!=0 && tipo!=1)) {
        fprintf(stderr,"Tipo de registo inválido.\n");
        limpar_stdin(); return;
    }
    limpar_stdin();

    struct tm d_tm = {0};
    d_tm.tm_mday=d; d_tm.tm_mon=m-1; d_tm.tm_year=y-1900;
    d_tm.tm_hour=hh; d_tm.tm_min=mm; d_tm.tm_sec=ss;

    int rc = registar_passagem(list, idSensor, codVeiculo, d_tm, ms, tipo);
    if (rc==0)             puts("Passagem registada.");
    else if (rc==-1)       puts("Data impossível.");
    else if (rc==-2)       puts("Nova data é anterior à última para o veículo.");
    else if (rc==-3)       puts("Parâmetros básicos inválidos.");
    else if (rc==-5)       puts("Milissegundos inválidos (deve ser 0-999).");
    else                   puts("Erro desconhecido.");
}

void salvar_passagens(PassagemList list, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Erro ao abrir arquivo de passagens");
        return;
    }

    PassagemNode* current = list;
    while (current != NULL) {
        struct tm* tm_info = localtime(&(current->passagem.data));
        char data_str[24];
        strftime(data_str, sizeof(data_str), "%d-%m-%Y %H:%M:%S", tm_info);
        
        fprintf(file, "%d\t%d\t%s.%03d\t%d\n",
                current->passagem.idSensor,
                current->passagem.codVeiculo,
                data_str,
                current->passagem.milissegundos,
                current->passagem.tipoRegisto);
        
        current = current->next;
    }

    fclose(file);
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

int comparar_passagem_por_data(const void* a, const void* b) {
    const Passagem* p1 = *(const Passagem**)a;
    const Passagem* p2 = *(const Passagem**)b;

    if (p1->data < p2->data) return -1;
    else if (p1->data > p2->data) return 1;
    else return 0;
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