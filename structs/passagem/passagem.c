#include "passagem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

typedef struct
{
    Veiculo *veiculo;
    float total_km;
} RankingItem;

typedef struct
{
    char marca[50];
    float total_km;
} RankingMarcaItem;

typedef struct {
    char matricula[16];
    float velocidade; // km/h
} InfracaoItem;

typedef struct {
    char matricula[16];
    int num_infracoes;
} RankingInfracaoItem;

typedef struct {
    char marca[32];
    float soma_velocidade;
    int count;
    float media;
} VelocidadeMarcaItem;

PassagemList criar_lista_passagens(void)
{
    return NULL;
}

<<<<<<< HEAD
void libertar_lista_passagens(PassagemList list) {
    PassagemNode* current = list;
    while (current) {
        PassagemNode* temp = current;
        current = current->next;
        free(temp);
=======
void liberar_lista_passagens(PassagemList list)
{
    while (list)
    {
        PassagemNode *tmp = list;
        list = list->next;
        free(tmp);
>>>>>>> origin/Duarte
    }
}

void inserir_passagem(PassagemList *list, Passagem p)
{
    PassagemNode *n = malloc(sizeof *n);
    if (!n)
    {
        perror("malloc inserir_passagem");
        return;
    }
    n->passagem = p;
    n->next = *list;
    *list = n;
}

static int parse_linha_passagem(const char *linha, Passagem *p)
{
    int d, m, y, hh, mm, ss, ms;
    int lidos = sscanf(linha,
                       "%d\t%d\t%d-%d-%d %d:%d:%d.%d\t%d",
                       &p->idSensor,
                       &p->codVeiculo,
                       &d, &m, &y,
                       &hh, &mm, &ss, &ms,
                       &p->tipoRegisto);

    if (lidos != 10)
        return -1;
    if (p->idSensor <= 0 || p->codVeiculo <= 0)
        return -2;
    if (p->tipoRegisto != 0 && p->tipoRegisto != 1)
        return -3;
    if (ms < 0 || ms > 999)
        return -5; // Novo erro para ms inválidos

    struct tm t = {0};
    t.tm_mday = d;
    t.tm_mon = m - 1;
    t.tm_year = y - 1900;
    t.tm_hour = hh;
    t.tm_min = mm;
    t.tm_sec = ss;

    time_t ts = mktime(&t);
    if (ts == (time_t)-1)
        return -4;

    p->data = ts;
    p->milissegundos = ms; // Armazena os milissegundos
    return 0;
}

void carregar_passagens(PassagemList *list, const char *ficheiro, int batch)
{
    FILE *f = fopen(ficheiro, "r");
    if (!f)
    {
        perror("carregar_passagens fopen");
        return;
    }

    char linha[256];
    long ok = 0, err_fmt = 0, err_neg = 0, err_tipo = 0, err_data = 0, err_ms = 0;

    while (fgets(linha, sizeof linha, f))
    {
        Passagem p;
        int rc = parse_linha_passagem(linha, &p);
        if (rc == 0)
        {
            inserir_passagem(list, p);
            if (++ok % batch == 0)
                printf("Carregadas %ld passagens...\n", ok);
        }
        else
        {
            if (rc == -1)
                err_fmt++;
            else if (rc == -2)
                err_neg++;
            else if (rc == -3)
                err_tipo++;
            else if (rc == -4)
                err_data++;
            else if (rc == -5)
                err_ms++;
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

int calcular_memoria_passagens(PassagemList list)
{
    int bytes = 0;
    for (PassagemNode *cur = list; cur; cur = cur->next)
        bytes += sizeof(PassagemNode);
    return bytes;
}

void listar_passagens_periodo(PassagemList list, time_t ini, time_t fim)
{
    for (PassagemNode *cur = list; cur; cur = cur->next)
    {
        if (cur->passagem.data < ini || cur->passagem.data > fim)
            continue;
        char buf[64];
        strftime(buf, sizeof buf, "%d-%m-%Y %H:%M:%S", localtime(&cur->passagem.data));
        printf("Sensor:%3d  Veículo:%5d  %s.%03d  %s\n",
               cur->passagem.idSensor,
               cur->passagem.codVeiculo,
               buf,
               cur->passagem.milissegundos,
               cur->passagem.tipoRegisto ? "Saída" : "Entrada");
    }
}

int registar_passagem(PassagemList *list,
                      int idSensor,
                      int codVeiculo,
                      struct tm data_tm,
                      int milissegundos,
                      int tipoRegisto)
{

    if (idSensor <= 0 || codVeiculo <= 0 || (tipoRegisto != 0 && tipoRegisto != 1))
        return -3;

    if (milissegundos < 0 || milissegundos > 999)
        return -5;

    time_t nova = mktime(&data_tm);
    if (nova == (time_t)-1)
        return -1;

    time_t ultima = 0;
    for (PassagemNode *cur = *list; cur; cur = cur->next)
    {
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
        .tipoRegisto = tipoRegisto};
    inserir_passagem(list, p);
    return 0;
}

static void limpar_stdin(void)
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
        ;
}

void registar_passagem_interativo(PassagemList *list)
{
    int idSensor, codVeiculo, tipo, ms;
    char str[128];

    puts("\n=== Registar Nova Passagem ===");

    printf("ID do sensor: ");
    if (scanf("%d", &idSensor) != 1 || idSensor <= 0)
    {
        fprintf(stderr, "Erro: ID do sensor inválido.\n");
        limpar_stdin();
        return;
    }

    printf("Código do veículo: ");
    if (scanf("%d", &codVeiculo) != 1 || codVeiculo <= 0)
    {
        fprintf(stderr, "Erro: código de veículo inválido.\n");
        limpar_stdin();
        return;
    }
    limpar_stdin();

    printf("Data e hora (dd-mm-aaaa HH:MM:SS.mmm): ");
    if (!fgets(str, sizeof str, stdin))
    {
        perror("fgets");
        return;
    }

    int d, m, y, hh, mm, ss;
    if (sscanf(str, "%d-%d-%d %d:%d:%d.%d", &d, &m, &y, &hh, &mm, &ss, &ms) != 7)
    {
        fprintf(stderr, "Formato de data/hora inválido.\n");
        return;
    }

    printf("Tipo de registo (0=entrada, 1=saída): ");
    if (scanf("%d", &tipo) != 1 || (tipo != 0 && tipo != 1))
    {
        fprintf(stderr, "Tipo de registo inválido.\n");
        limpar_stdin();
        return;
    }
    limpar_stdin();

    struct tm d_tm = {0};
    d_tm.tm_mday = d;
    d_tm.tm_mon = m - 1;
    d_tm.tm_year = y - 1900;
    d_tm.tm_hour = hh;
    d_tm.tm_min = mm;
    d_tm.tm_sec = ss;

    int rc = registar_passagem(list, idSensor, codVeiculo, d_tm, ms, tipo);
    if (rc == 0)
        puts("Passagem registada.");
    else if (rc == -1)
        puts("Data impossível.");
    else if (rc == -2)
        puts("Nova data é anterior à última para o veículo.");
    else if (rc == -3)
        puts("Parâmetros básicos inválidos.");
    else if (rc == -5)
        puts("Milissegundos inválidos (deve ser 0-999).");
    else
        puts("Erro desconhecido.");
}

void salvar_passagens(PassagemList list, const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        perror("Erro ao abrir arquivo de passagens");
        return;
    }

    PassagemNode *current = list;
    while (current != NULL)
    {
        struct tm *tm_info = localtime(&(current->passagem.data));
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

void listar_veiculos_periodo(PassagemList passagens, VeiculoHashTable *veiculos, time_t inicio, time_t fim)
{
    if (!passagens || !veiculos)
        return;

    void **veiculos_periodo = NULL;
    int count = 0;
    int capacity = 0;

    PassagemNode *current = passagens;
    while (current)
    {
        if (current->passagem.data >= inicio && current->passagem.data <= fim)
        {
            Veiculo *veiculo = buscar_veiculo_codigo(veiculos, current->passagem.codVeiculo);

            if (veiculo)
            {
                // Verifica se já está no array
                int encontrado = 0;
                for (int i = 0; i < count; i++)
                {
                    if (veiculos_periodo[i] == (void *)veiculo)
                    {
                        encontrado = 1;
                        break;
                    }
                }

                if (!encontrado)
                {
                    if (count >= capacity)
                    {
                        capacity = (capacity == 0) ? 16 : capacity * 2;
                        void **temp = realloc(veiculos_periodo, capacity * sizeof(void *));
                        if (!temp)
                        {
                            perror("Erro ao realocar memória");
                            free(veiculos_periodo);
                            return;
                        }
                        veiculos_periodo = temp;
                    }
                    veiculos_periodo[count++] = (void *)veiculo;
                }
            }
        }
        current = current->next;
    }

    if (count == 0)
    {
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

<<<<<<< HEAD
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
        
=======
void mostrarRankingKmPVeiculo(void *dado)
{
    RankingItem *ptr;
    ptr = (RankingItem *)dado;
    Veiculo *v = ptr->veiculo;
    printf("%-10s %-15s %-15s %-4d %-10.2f %s\n",
           v->matricula, v->marca, v->modelo, v->ano,
           ptr->total_km,
           v->dono ? v->dono->nome : "N/A");
}

void mostrarInfracao(void *item) {
    InfracaoItem *inf = (InfracaoItem *)item;
    printf("%-15s %8.2f km/h\n", inf->matricula, inf->velocidade);
}

void mostrarRankingInfracao(void *item) {
    RankingInfracaoItem *r = (RankingInfracaoItem *)item;
    printf("%-15s %10d\n", r->matricula, r->num_infracoes);
}

void mostrarRankingMarca(void *dado)
{
    RankingMarcaItem *ptr = (RankingMarcaItem *)dado;
    printf("%-15s %-10.2f\n", ptr->marca, ptr->total_km);
}

void mostrarMarcaMaiorVelocidadeMedia(VelocidadeMarcaItem *item) {
    printf("%-20s %.2f km/h\n", item->marca, item->media);
}

int comparar_passagem_por_data(const void *a, const void *b)
{
    const Passagem *p1 = *(const Passagem **)a;
    const Passagem *p2 = *(const Passagem **)b;

    if (p1->data < p2->data)
        return -1;
    else if (p1->data > p2->data)
        return 1;
    else
        return 0;
}

int comparar_ranking_item_por_km(const void *a, const void *b) {
    const RankingMarcaItem *itemA = (const RankingMarcaItem *)a;
    const RankingMarcaItem *itemB = (const RankingMarcaItem *)b;

    if (itemA->total_km < itemB->total_km) return 1;
    if (itemA->total_km > itemB->total_km) return -1;
    return 0;
}


void listar_ranking_circulacao(PassagemList passagens, VeiculoHashTable *veiculos, DistanciaList distancias, time_t inicio, time_t fim)
{
    if (!passagens || !veiculos)
        return;

    RankingItem *ranking = NULL;
    int count = 0, capacity = 0;

    // Etapa 1: Agrupar veículos únicos no período
    for (PassagemNode *current = passagens; current; current = current->next)
    {
        if (current->passagem.data < inicio || current->passagem.data > fim)
            continue;

        Veiculo *veiculo = buscar_veiculo_codigo(veiculos, current->passagem.codVeiculo);
        if (!veiculo)
            continue;

        int i, encontrado = 0;
        for (i = 0; i < count; i++)
        {
            if (ranking[i].veiculo == veiculo)
            {
                encontrado = 1;
                break;
            }
        }

        if (!encontrado)
        {
            if (count >= capacity)
            {
                capacity = (capacity == 0) ? 16 : capacity * 2;
                RankingItem *temp = realloc(ranking, capacity * sizeof(RankingItem));
                if (!temp)
                {
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
    for (int i = 0; i < count; i++)
    {
        Passagem **p_array = NULL;
        int p_count = 0, p_cap = 0;

        // Recolher passagens desse veículo
        for (PassagemNode *node = passagens; node; node = node->next)
        {
            if (node->passagem.codVeiculo == ranking[i].veiculo->codVeiculo &&
                node->passagem.data >= inicio && node->passagem.data <= fim)
            {

                if (p_count >= p_cap)
                {
                    p_cap = (p_cap == 0) ? 8 : p_cap * 2;
                    Passagem **temp = realloc(p_array, p_cap * sizeof(Passagem *));
                    if (!temp)
                        break;
                    p_array = temp;
                }
                p_array[p_count++] = &node->passagem;
            }
        }

        // Ordenar passagens por data
        qsort(p_array, p_count, sizeof(Passagem *), comparar_passagem_por_data);

        // Calcular distâncias entre pares entrada/saída
        for (int j = 0; j < p_count - 1; j++)
        {
            if (p_array[j]->tipoRegisto == 0 && p_array[j + 1]->tipoRegisto == 1)
            {
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
    void **ranking_ptrs = malloc(count * sizeof(void *));
    if (!ranking_ptrs)
    {
        perror("Erro ao alocar memória para ponteiros do ranking");
        free(ranking);
        return;
    }
    for (int i = 0; i < count; i++)
    {
        ranking_ptrs[i] = &ranking[i];
    }

    // Etapa 5: Mostrar com paginacao
    printf("\n=== Ranking de circulação no período ===\n");
    printf("%-10s %-15s %-15s %-4s %-10s %s\n", "Matrícula", "Marca", "Modelo", "Ano", "KM", "Dono");
    paginacao(ranking_ptrs, count, 30, mostrarRankingKmPVeiculo);

    free(ranking_ptrs);
    free(ranking);
}

void listar_ranking_marca(PassagemList passagens, VeiculoHashTable *veiculos, DistanciaList distancias, time_t inicio, time_t fim)
{
    if (!passagens || !veiculos)
        return;

    RankingMarcaItem *ranking = NULL;
    int count = 0, capacity = 0;

    // Etapa 1: Agrupar marcas únicas no período
    for (PassagemNode *current = passagens; current; current = current->next)
    {
        if (current->passagem.data < inicio || current->passagem.data > fim)
            continue;

        Veiculo *veiculo = buscar_veiculo_codigo(veiculos, current->passagem.codVeiculo);
        if (!veiculo)
            continue;

        int i, encontrado = 0;
        for (i = 0; i < count; i++)
        {
            if (strcmp(ranking[i].marca, veiculo->marca) == 0)
            {
                encontrado = 1;
                break;
            }
        }

        if (!encontrado)
        {
            if (count >= capacity)
            {
                capacity = (capacity == 0) ? 16 : capacity * 2;
                RankingMarcaItem *temp = realloc(ranking, capacity * sizeof(RankingMarcaItem));
                if (!temp)
                {
                    perror("Erro ao realocar memória");
                    free(ranking);
                    return;
                }
                ranking = temp;
            }
            strcpy(ranking[count].marca, veiculo->marca);
            ranking[count].total_km = 0.0f;
            count++;
        }
    }
    // Etapa 2: Para cada marca, calcular total de km
    for (PassagemNode *current = passagens; current; current = current->next)
    {
        if (current->passagem.data < inicio || current->passagem.data > fim)
            continue;

        Veiculo *veiculo = buscar_veiculo_codigo(veiculos, current->passagem.codVeiculo);
        if (!veiculo)
            continue;

        for (int i = 0; i < count; i++)
        {
            if (strcmp(ranking[i].marca, veiculo->marca) == 0)
            {
                // Recolher passagens desse veículo
                Passagem **p_array = NULL;
                int p_count = 0, p_cap = 0;

                for (PassagemNode *node = passagens; node; node = node->next)
                {
                    if (node->passagem.codVeiculo == veiculo->codVeiculo &&
                        node->passagem.data >= inicio && node->passagem.data <= fim)
                    {

                        if (p_count >= p_cap)
                        {
                            p_cap = (p_cap == 0) ? 8 : p_cap * 2;
                            Passagem **temp = realloc(p_array, p_cap * sizeof(Passagem *));
                            if (!temp)
                                break;
                            p_array = temp;
                        }
                        p_array[p_count++] = &node->passagem;
                    }
                }

                // Ordenar passagens por data
                qsort(p_array, p_count, sizeof(Passagem *), comparar_passagem_por_data);

                // Calcular distâncias entre pares entrada/saída
                for (int j = 0; j < p_count - 1; j++)
                {
                    if (p_array[j]->tipoRegisto == 0 && p_array[j + 1]->tipoRegisto == 1)
                    {
                        float d = obter_distancia_entre_sensores(distancias, p_array[j]->idSensor, p_array[j + 1]->idSensor);
                        ranking[i].total_km += d;
                        j++; // saltar o par usado
                    }
                }

                free(p_array);
                break; // Marca encontrada, sair do loop
            }
        }
    }
    // Etapa 3: Ordenar por total_km decrescente
    qsort(ranking, count, sizeof(RankingMarcaItem), comparar_ranking_item_por_km);

    // Etapa 4: Criar array de ponteiros para passar à paginacao
    void **ranking_ptrs = malloc(count * sizeof(void *));
    for (int i = 0; i < count; i++)
    {
        ranking_ptrs[i] = &ranking[i];
    }
    // Etapa 5: Mostrar com paginacao
    printf("\n=== Ranking por marca no período ===\n");
    printf("%-15s %-10s\n", "Marca", "Total KM");
    paginacao(ranking_ptrs, count, 30, mostrarRankingMarca);
    free(ranking_ptrs);
    free(ranking);
}

void listar_infracoes(PassagemList passagens, VeiculoHashTable *veiculos, DistanciaList distancias, time_t inicio, time_t fim)
{
    if (!passagens || !veiculos)
        return;

    InfracaoItem *infracoes = NULL;
    int count = 0, capacity = 0;

    // Etapa 1: Percorrer veículos
    for (PassagemNode *current = passagens; current; current = current->next)
    {
        if (current->passagem.data < inicio || current->passagem.data > fim)
            continue;

        Veiculo *veiculo = buscar_veiculo_codigo(veiculos, current->passagem.codVeiculo);
        if (!veiculo)
            continue;

        // Verificar se já analisamos este veículo
        int ja_existe = 0;
        for (int i = 0; i < count; i++)
        {
            if (strcmp(infracoes[i].matricula, veiculo->matricula) == 0)
            {
                ja_existe = 1;
                break;
            }
        }
        if (ja_existe)
            continue;

        // Coletar passagens desse veículo no intervalo
        Passagem **p_array = NULL;
        int p_count = 0, p_cap = 0;

        for (PassagemNode *node = passagens; node; node = node->next)
        {
            if (node->passagem.codVeiculo == veiculo->codVeiculo &&
                node->passagem.data >= inicio && node->passagem.data <= fim)
            {
                if (p_count >= p_cap)
                {
                    p_cap = (p_cap == 0) ? 8 : p_cap * 2;
                    Passagem **temp = realloc(p_array, p_cap * sizeof(Passagem *));
                    if (!temp)
                        break;
                    p_array = temp;
                }
                p_array[p_count++] = &node->passagem;
            }
        }

        // Ordenar por data
        qsort(p_array, p_count, sizeof(Passagem *), comparar_passagem_por_data);

        // Calcular velocidades entre pares
        for (int i = 0; i < p_count - 1; i++)
        {
            if (p_array[i]->tipoRegisto == 0 && p_array[i + 1]->tipoRegisto == 1)
            {
                float distancia_km = obter_distancia_entre_sensores(distancias, p_array[i]->idSensor, p_array[i + 1]->idSensor);
                time_t delta_t = difftime(p_array[i + 1]->data, p_array[i]->data); // segundos

                if (delta_t <= 0)
                    continue;

                float horas = delta_t / 3600.0f;
                float velocidade = distancia_km / horas;

                if (velocidade > 120.0f)
                {
                    // Adicionar à lista de infrações
                    if (count >= capacity)
                    {
                        capacity = (capacity == 0) ? 8 : capacity * 2;
                        InfracaoItem *temp = realloc(infracoes, capacity * sizeof(InfracaoItem));
                        if (!temp)
                        {
                            perror("Erro ao alocar memória");
                            free(infracoes);
                            free(p_array);
                            return;
                        }
                        infracoes = temp;
                    }

                    strcpy(infracoes[count].matricula, veiculo->matricula);
                    infracoes[count].velocidade = velocidade;
                    count++;
                    break; // Só precisa de uma infração por veículo
                }

                i++; // Avançar para além do par usado
            }
        }

        free(p_array);
    }

    // Etapa 2: Mostrar resultados com paginação
    printf("\n=== Lista de Infrações por Velocidade > 120 km/h ===\n");

    if (count == 0)
    {
        printf("Nenhuma infração registrada no período.\n");
    }
    else
    {
        printf("%-15s %-10s\n", "Matrícula", "Velocidade");

        // Criar array de ponteiros para a paginação
        void **infracoes_ptrs = malloc(count * sizeof(void *));
        for (int i = 0; i < count; i++)
        {
            infracoes_ptrs[i] = &infracoes[i];
        }

        paginacao(infracoes_ptrs, count, 30, mostrarInfracao);

        free(infracoes_ptrs);
    }

    free(infracoes);
}

int comparar_ranking_infracao(const void *a, const void *b)
{
    const RankingInfracaoItem *r1 = (const RankingInfracaoItem *)a;
    const RankingInfracaoItem *r2 = (const RankingInfracaoItem *)b;
    return r2->num_infracoes - r1->num_infracoes; // ordem decrescente
}


void listar_ranking_infracoes(PassagemList passagens, VeiculoHashTable *veiculos, DistanciaList distancias, time_t inicio, time_t fim)
{
    if (!passagens || !veiculos)
        return;

    RankingInfracaoItem *ranking = NULL;
    int count = 0, capacity = 0;

    // Etapa 1: Para cada veículo que aparece no período, agrupar passagens e contar infrações
    for (PassagemNode *current = passagens; current; current = current->next)
    {
        if (current->passagem.data < inicio || current->passagem.data > fim)
            continue;

        Veiculo *veiculo = buscar_veiculo_codigo(veiculos, current->passagem.codVeiculo);
        if (!veiculo)
            continue;

        // Já analisado?
        int idx = -1;
        for (int i = 0; i < count; i++)
        {
            if (strcmp(ranking[i].matricula, veiculo->matricula) == 0)
            {
                idx = i;
                break;
            }
        }

        if (idx == -1)
        {
            // Novo veículo
            if (count >= capacity)
            {
                capacity = (capacity == 0) ? 8 : capacity * 2;
                RankingInfracaoItem *temp = realloc(ranking, capacity * sizeof(RankingInfracaoItem));
                if (!temp)
                {
                    perror("Erro ao alocar memória");
                    free(ranking);
                    return;
                }
                ranking = temp;
            }

            strcpy(ranking[count].matricula, veiculo->matricula);
            ranking[count].num_infracoes = 0;
            idx = count++;
        }

        // Coletar passagens do veículo
        Passagem **p_array = NULL;
        int p_count = 0, p_cap = 0;

        for (PassagemNode *node = passagens; node; node = node->next)
        {
            if (node->passagem.codVeiculo == veiculo->codVeiculo &&
                node->passagem.data >= inicio && node->passagem.data <= fim)
            {
                if (p_count >= p_cap)
                {
                    p_cap = (p_cap == 0) ? 8 : p_cap * 2;
                    Passagem **temp = realloc(p_array, p_cap * sizeof(Passagem *));
                    if (!temp)
                        break;
                    p_array = temp;
                }
                p_array[p_count++] = &node->passagem;
            }
        }

        // Ordenar por data
        qsort(p_array, p_count, sizeof(Passagem *), comparar_passagem_por_data);

        // Verificar pares entrada/saída e contar infrações
        for (int i = 0; i < p_count - 1; i++)
        {
            if (p_array[i]->tipoRegisto == 0 && p_array[i + 1]->tipoRegisto == 1)
            {
                float dist_km = obter_distancia_entre_sensores(distancias, p_array[i]->idSensor, p_array[i + 1]->idSensor);
                time_t delta_t = difftime(p_array[i + 1]->data, p_array[i]->data);

                if (delta_t <= 0)
                    continue;

                float horas = delta_t / 3600.0f;
                float velocidade = dist_km / horas;

                if (velocidade > 120.0f)
                    ranking[idx].num_infracoes++;

                i++; // saltar o par
            }
        }

        free(p_array);
    }

    qsort(ranking, count, sizeof(RankingInfracaoItem), comparar_ranking_infracao);

    // Etapa 3: Mostrar ranking
    // Criar array de ponteiros para passar à paginacao
    void **ranking_ptrs = malloc(count * sizeof(void *));
    for (int i = 0; i < count; i++) {
        ranking_ptrs[i] = &ranking[i];
    }

    printf("\n=== Ranking de Infrações por Veículo ===\n");
    printf("%-15s %-10s\n", "Matrícula", "Infrações");

    // Chamar a paginacao
    paginacao(ranking_ptrs, count, 30, mostrarRankingInfracao);

    free(ranking_ptrs);
    free(ranking);
}

void listar_marca_maior_velocidade_media(PassagemList passagens, VeiculoHashTable *veiculos, DistanciaList distancias)
{
    if (!passagens || !veiculos)
        return;

    VelocidadeMarcaItem *velocidades = NULL;
    int count = 0, capacity = 0;

    for (PassagemNode *current = passagens; current; current = current->next)
    {
        Veiculo *veiculo = buscar_veiculo_codigo(veiculos, current->passagem.codVeiculo);
        if (!veiculo)
            continue;

        // Procurar ou criar entrada da marca
        int idx = -1;
        for (int i = 0; i < count; i++)
        {
            if (strcmp(velocidades[i].marca, veiculo->marca) == 0)
            {
                idx = i;
                break;
            }
        }

        if (idx == -1)
        {
            if (count >= capacity)
            {
                capacity = (capacity == 0) ? 8 : capacity * 2;
                VelocidadeMarcaItem *temp = realloc(velocidades, capacity * sizeof(VelocidadeMarcaItem));
                if (!temp)
                {
                    perror("Erro ao alocar memória");
                    free(velocidades);
                    return;
                }
                velocidades = temp;
            }
            strcpy(velocidades[count].marca, veiculo->marca);
            velocidades[count].soma_velocidade = 0.0f;
            velocidades[count].count = 0;
            velocidades[count].media = 0.0f;
            idx = count++;
        }

        // Recolher todas as passagens do veículo (no geral)
        Passagem **p_array = NULL;
        int p_count = 0, p_cap = 0;

        for (PassagemNode *node = passagens; node; node = node->next)
        {
            if (node->passagem.codVeiculo == veiculo->codVeiculo)
            {
                if (p_count >= p_cap)
                {
                    p_cap = (p_cap == 0) ? 8 : p_cap * 2;
                    Passagem **temp = realloc(p_array, p_cap * sizeof(Passagem *));
                    if (!temp)
                        break;
                    p_array = temp;
                }
                p_array[p_count++] = &node->passagem;
            }
        }

        // Ordenar por data
        qsort(p_array, p_count, sizeof(Passagem *), comparar_passagem_por_data);

        // Calcular velocidades por pares entrada/saída
        for (int i = 0; i < p_count - 1; i++)
        {
            if (p_array[i]->tipoRegisto == 0 && p_array[i + 1]->tipoRegisto == 1)
            {
                float distancia_km = obter_distancia_entre_sensores(distancias, p_array[i]->idSensor, p_array[i + 1]->idSensor);
                time_t delta_t = difftime(p_array[i + 1]->data, p_array[i]->data);
                if (delta_t <= 0)
                    continue;

                float horas = delta_t / 3600.0f;
                float velocidade = distancia_km / horas;

                velocidades[idx].soma_velocidade += velocidade;
                velocidades[idx].count++;
                i++; // saltar o par usado
            }
        }

        free(p_array);
    }

    if (count == 0)
    {
        printf("Nenhuma passagem disponível.\n");
        free(velocidades);
        return;
    }

    // Calcular médias por marca e encontrar a maior
    float maior_media = 0.0f;
    int indice_maior = -1;
    for (int i = 0; i < count; i++)
    {
        if (velocidades[i].count > 0)
        {
            velocidades[i].media = velocidades[i].soma_velocidade / velocidades[i].count;
            if (velocidades[i].media > maior_media)
            {
                maior_media = velocidades[i].media;
                indice_maior = i;
            }
        }
    }

    // Mostrar apenas a marca com maior média
    if (indice_maior >= 0)
    {
        printf("\n=== Marca com Maior Velocidade Média ===\n");
        printf("%-20s %-10s\n", "Marca", "Velocidade Média");
        printf("%-20s %.2f km/h\n", velocidades[indice_maior].marca, velocidades[indice_maior].media);
    }
    else
    {
        printf("Nenhuma velocidade média válida calculada.\n");
    }

    free(velocidades);
}


void exportPassagemToCSV(PassagemList list, const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (!file)
    {
        perror("Erro ao abrir arquivo");
        return;
    }

    fprintf(file, "idSensor,codVeiculo,data,milissegundos,tipoRegisto\n");

    PassagemNode *current = list;
    while (current != NULL)
    {
        char dateStr[20];
        strftime(dateStr, sizeof(dateStr), "%Y-%m-%d %H:%M:%S", localtime(&current->passagem.data));

        fprintf(file, "%d,%d,%s,%d,%d\n",
                current->passagem.idSensor,
                current->passagem.codVeiculo,
                dateStr,
                current->passagem.milissegundos,
                current->passagem.tipoRegisto);
>>>>>>> origin/Duarte
        current = current->next;
    }

    fclose(file);
<<<<<<< HEAD
    printf("Passagens salvas com sucesso em %s\n", filename);
=======
>>>>>>> origin/Duarte
}