/**
 * \file passagem.c
 * \brief Implementação das funções de manipulação de passagens de veículos.
 */

#include "passagem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

/**
 * \struct RankingItem
 * \brief Estrutura auxiliar para ranking de veículos por quilometragem.
 */
typedef struct
{
    Veiculo *veiculo;      /**< Ponteiro para o veículo */
    float total_km;        /**< Total de km percorridos */
} RankingItem;

/**
 * \struct RankingMarcaItem
 * \brief Estrutura auxiliar para ranking de marcas por quilometragem.
 */
typedef struct
{
    char marca[50];        /**< Nome da marca */
    float total_km;        /**< Total de km percorridos */
} RankingMarcaItem;

/**
 * \struct InfracaoItem
 * \brief Estrutura para armazenar infrações de velocidade.
 */
typedef struct {
    char matricula[16];    /**< Matrícula do veículo */
    float velocidade;      /**< Velocidade em km/h */
} InfracaoItem;

/**
 * \struct RankingInfracaoItem
 * \brief Estrutura para ranking de veículos por número de infrações.
 */
typedef struct {
    char matricula[16];    /**< Matrícula do veículo */
    int num_infracoes;     /**< Número de infrações */
} RankingInfracaoItem;

/**
 * \struct VelocidadeMarcaItem
 * \brief Estrutura para cálculo da velocidade média por marca.
 */
typedef struct {
    char marca[32];        /**< Nome da marca */
    float soma_velocidade; /**< Soma das velocidades */
    int count;             /**< Número de ocorrências */
    float media;           /**< Velocidade média */
} VelocidadeMarcaItem;

/**
 * \struct VelocidadeDonoItem
 * \brief Estrutura para cálculo da velocidade média por dono.
 */
typedef struct {
    Dono *dono;            /**< Ponteiro para o dono */
    float soma_velocidade; /**< Soma das velocidades */
    int count;             /**< Número de ocorrências */
    float media;           /**< Velocidade média */
} VelocidadeDonoItem;

/**
 * \struct VelocidadeVeiculoItem
 * \brief Estrutura para cálculo da velocidade média por veículo.
 */
typedef struct {
    Veiculo *veiculo;      /**< Ponteiro para o veículo */
    float soma_velocidade; /**< Soma das velocidades */
    int count;             /**< Número de ocorrências */
    float media;           /**< Velocidade média */
} VelocidadeVeiculoItem;

/**
 * \brief Cria uma lista de passagens vazia.
 * \return Lista vazia (NULL).
 */
PassagemList criar_lista_passagens(void)
{
    return NULL;
}

/**
 * \brief Libera toda a memória associada à lista de passagens.
 * \param list Lista de passagens a ser liberada.
 */
void liberar_lista_passagens(PassagemList list)
{
    while (list)
    {
        PassagemNode *tmp = list;
        list = list->next;
        free(tmp);
    }
}

/**
 * \brief Insere uma passagem na lista.
 * \param list Ponteiro para a lista de passagens.
 * \param p Passagem a ser inserida.
 */
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

/**
 * \brief Analisa uma linha de texto e preenche uma estrutura Passagem.
 * \param linha Linha de texto.
 * \param p Ponteiro para estrutura Passagem a ser preenchida.
 * \return 0 se sucesso, valor negativo se erro.
 */
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

/**
 * \brief Carrega passagens de um ficheiro.
 * \param list Ponteiro para a lista de passagens.
 * \param ficheiro Nome do ficheiro.
 * \param batch Número de passagens para mostrar progresso.
 */
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

/**
 * \brief Calcula a memória ocupada pela lista de passagens.
 * \param list Lista de passagens.
 * \return Memória em bytes.
 */
int calcular_memoria_passagens(PassagemList list)
{
    int bytes = 0;
    for (PassagemNode *cur = list; cur; cur = cur->next)
        bytes += sizeof(PassagemNode);
    return bytes;
}

/**
 * \brief Lista passagens num determinado período.
 * \param list Lista de passagens.
 * \param ini Data/hora inicial.
 * \param fim Data/hora final.
 */
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

/**
 * \brief Regista uma nova passagem na lista.
 * \param list Ponteiro para a lista.
 * \param idSensor ID do sensor.
 * \param codVeiculo Código do veículo.
 * \param data_tm Estrutura tm com data/hora.
 * \param milissegundos Milissegundos.
 * \param tipoRegisto Tipo de registo (0=entrada, 1=saída).
 * \return 0 se sucesso, valor negativo se erro.
 */
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

/**
 * \brief Regista uma passagem de forma interativa via terminal.
 * \param list Ponteiro para a lista.
 */
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

/**
 * \brief Salva as passagens num ficheiro.
 * \param list Lista de passagens.
 * \param filename Nome do ficheiro.
 */
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

/**
 * \brief Lista veículos que passaram num período.
 * \param passagens Lista de passagens.
 * \param veiculos Tabela hash de veículos.
 * \param inicio Data/hora inicial.
 * \param fim Data/hora final.
 */
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

/**
 * \brief Mostra um item do ranking de km por veículo.
 * \param dado Ponteiro para RankingItem.
 */
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

/**
 * \brief Mostra uma infração de velocidade.
 * \param item Ponteiro para InfracaoItem.
 */
void mostrarInfracao(void *item) {
    InfracaoItem *inf = (InfracaoItem *)item;
    printf("%-15s %8.2f km/h\n", inf->matricula, inf->velocidade);
}

/**
 * \brief Mostra um item do ranking de infrações.
 * \param item Ponteiro para RankingInfracaoItem.
 */
void mostrarRankingInfracao(void *item) {
    RankingInfracaoItem *r = (RankingInfracaoItem *)item;
    printf("%-15s %10d\n", r->matricula, r->num_infracoes);
}

/**
 * \brief Mostra um item do ranking de marcas.
 * \param dado Ponteiro para RankingMarcaItem.
 */
void mostrarRankingMarca(void *dado)
{
    RankingMarcaItem *ptr = (RankingMarcaItem *)dado;
    printf("%-15s %-10.2f\n", ptr->marca, ptr->total_km);
}

/**
 * \brief Mostra a marca com maior velocidade média.
 * \param item Ponteiro para VelocidadeMarcaItem.
 */
void mostrarMarcaMaiorVelocidadeMedia(VelocidadeMarcaItem *item) {
    printf("%-20s %.2f km/h\n", item->marca, item->media);
}

/**
 * \brief Compara duas passagens por data.
 * \param a Ponteiro para ponteiro de Passagem.
 * \param b Ponteiro para ponteiro de Passagem.
 * \return -1, 0 ou 1 conforme a ordem.
 */
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

/**
 * \brief Compara dois itens de ranking de marca por km.
 * \param a Ponteiro para RankingMarcaItem.
 * \param b Ponteiro para RankingMarcaItem.
 * \return 1, -1 ou 0.
 */
int comparar_ranking_item_por_km(const void *a, const void *b) {
    const RankingMarcaItem *itemA = (const RankingMarcaItem *)a;
    const RankingMarcaItem *itemB = (const RankingMarcaItem *)b;

    if (itemA->total_km < itemB->total_km) return 1;
    if (itemA->total_km > itemB->total_km) return -1;
    return 0;
}

/**
 * \brief Lista ranking de circulação por veículo.
 * \param passagens Lista de passagens.
 * \param veiculos Tabela hash de veículos.
 * \param distancias Lista de distâncias.
 * \param inicio Data/hora inicial.
 * \param fim Data/hora final.
 */
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

/**
 * \brief Lista ranking de circulação por marca.
 * \param passagens Lista de passagens.
 * \param veiculos Tabela hash de veículos.
 * \param distancias Lista de distâncias.
 * \param inicio Data/hora inicial.
 * \param fim Data/hora final.
 */
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

/**
 * \brief Lista infrações de velocidade (>120 km/h) no período.
 * \param passagens Lista de passagens.
 * \param veiculos Tabela hash de veículos.
 * \param distancias Lista de distâncias.
 * \param inicio Data/hora inicial.
 * \param fim Data/hora final.
 */
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

                float horas = delta_t / 3600000.0f;
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

/**
 * \brief Compara dois itens de ranking de infrações.
 * \param a Ponteiro para RankingInfracaoItem.
 * \param b Ponteiro para RankingInfracaoItem.
 * \return Diferença do número de infrações.
 */
int comparar_ranking_infracao(const void *a, const void *b)
{
    const RankingInfracaoItem *r1 = (const RankingInfracaoItem *)a;
    const RankingInfracaoItem *r2 = (const RankingInfracaoItem *)b;
    return r2->num_infracoes - r1->num_infracoes; // ordem decrescente
}

/**
 * \brief Lista ranking de infrações por veículo.
 * \param passagens Lista de passagens.
 * \param veiculos Tabela hash de veículos.
 * \param distancias Lista de distâncias.
 * \param inicio Data/hora inicial.
 * \param fim Data/hora final.
 */
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

                float horas = delta_t / 3600000.0f;
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

/**
 * \brief Lista a marca com maior velocidade média.
 * \param passagens Lista de passagens.
 * \param veiculos Tabela hash de veículos.
 * \param distancias Lista de distâncias.
 */
void listar_marca_maior_velocidade_media(PassagemList passagens, VeiculoHashTable *veiculos, DistanciaList distancias)
{
    if (!passagens || !veiculos)
        return;

    VelocidadeMarcaItem *velocidades = NULL;
    int count = 0, capacity = 0;

    // Percorrer TODAS as passagens e calcular velocidades entre pares válidos
    PassagemNode *current = passagens;
    while (current && current->next)
    {
        Passagem *p1 = &current->passagem;
        Passagem *p2 = &current->next->passagem;

        // Procurar par entrada/saída
        if (p1->codVeiculo == p2->codVeiculo &&
            p1->tipoRegisto == 0 && p2->tipoRegisto == 1)
        {
            Veiculo *veiculo = buscar_veiculo_codigo(veiculos, p1->codVeiculo);
            if (!veiculo)
            {
                current = current->next;
                continue;
            }

            float distancia_km = obter_distancia_entre_sensores(distancias, p1->idSensor, p2->idSensor);
            time_t delta = difftime(p2->data, p1->data);
            if (delta <= 0 || distancia_km <= 0)
            {
                current = current->next;
                continue;
            }

            float horas = delta / 3600000.0f; // 
            float velocidade = distancia_km / horas;

            //printf("Debug D: %f ! T: %f", distancia_km, horas);
            //getc(stdin);

            // Procurar ou criar item da marca
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

            velocidades[idx].soma_velocidade += velocidade;
            velocidades[idx].count++;
            current = current->next->next; // saltar par usado
        }
        else
        {
            current = current->next;
        }
    }

    // Calcular médias e encontrar maior
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

    // Mostrar resultado
    if (indice_maior >= 0)
    {
        printf("\n=== Marca com Maior Velocidade Média ===\n");
        printf("%-20s %-20s\n", "Marca", "Velocidade Média");
        printf("%-20s %8.2f km/h\n", velocidades[indice_maior].marca, velocidades[indice_maior].media);
    }
    else
    {
        printf("Nenhuma velocidade válida encontrada.\n");
    }

    free(velocidades);
}

/**
 * \brief Lista o dono com maior velocidade média.
 * \param passagens Lista de passagens.
 * \param veiculos Tabela hash de veículos.
 * \param distancias Lista de distâncias.
 */
void listar_dono_maior_velocidade_media(PassagemList passagens, VeiculoHashTable *veiculos, DistanciaList distancias)
{
    if (!passagens || !veiculos)
        return;

    VelocidadeDonoItem *velocidades = NULL;
    int count = 0, capacity = 0;

    PassagemNode *current = passagens;
    while (current && current->next)
    {
        Passagem *p1 = &current->passagem;
        Passagem *p2 = &current->next->passagem;

        if (p1->codVeiculo == p2->codVeiculo &&
            p1->tipoRegisto == 0 && p2->tipoRegisto == 1)
        {
            Veiculo *veiculo = buscar_veiculo_codigo(veiculos, p1->codVeiculo);
            if (!veiculo || !veiculo->dono)
            {
                current = current->next;
                continue;
            }

            float distancia = obter_distancia_entre_sensores(distancias, p1->idSensor, p2->idSensor);
            time_t delta = difftime(p2->data, p1->data);
            if (delta <= 0 || distancia <= 0)
            {
                current = current->next;
                continue;
            }

            float horas = delta / 3600000.0f;
            float velocidade = distancia / horas;

            // Verifica se já existe esse dono no array
            int idx = -1;
            for (int i = 0; i < count; i++)
            {
                if (velocidades[i].dono->numContribuinte == veiculo->dono->numContribuinte)
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
                    VelocidadeDonoItem *temp = realloc(velocidades, capacity * sizeof(VelocidadeDonoItem));
                    if (!temp)
                    {
                        perror("Erro ao alocar memória");
                        free(velocidades);
                        return;
                    }
                    velocidades = temp;
                }

                velocidades[count].dono = veiculo->dono;
                velocidades[count].soma_velocidade = 0.0f;
                velocidades[count].count = 0;
                velocidades[count].media = 0.0f;
                idx = count++;
            }

            velocidades[idx].soma_velocidade += velocidade;
            velocidades[idx].count++;

            current = current->next->next;
        }
        else
        {
            current = current->next;
        }
    }

    // Calcular médias
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

    // Mostrar resultado
    if (indice_maior >= 0)
    {
        Dono *d = velocidades[indice_maior].dono;
        printf("\n=== Dono com Maior Velocidade Média ===\n");
        printf("Nome: %s\n", d->nome);
        printf("NIF : %d\n", d->numContribuinte);
        printf("Velocidade Média: %.2f km/h\n", velocidades[indice_maior].media);
    }
    else
    {
        printf("Nenhuma velocidade válida foi calculada.\n");
    }

    free(velocidades);
}

/**
 * \brief Mostra o veículo mais rápido.
 * \param passagens Lista de passagens.
 * \param veiculos Tabela hash de veículos.
 * \param distancias Lista de distâncias.
 */
void mostrar_veiculo_mais_rapido(PassagemList passagens, VeiculoHashTable *veiculos, DistanciaList distancias)
{
    if (!passagens || !veiculos)
        return;

    VelocidadeVeiculoItem *ranking = NULL;
    int count = 0, cap = 0;

    // Agrupar velocidades por veículo
    PassagemNode *current = passagens;
    while (current && current->next)
    {
        Passagem *p1 = &current->passagem;
        Passagem *p2 = &current->next->passagem;

        if (p1->codVeiculo == p2->codVeiculo &&
            p1->tipoRegisto == 0 && p2->tipoRegisto == 1)
        {
            Veiculo *v = buscar_veiculo_codigo(veiculos, p1->codVeiculo);
            if (!v || !v->dono)
            {
                current = current->next;
                continue;
            }

            float dist = obter_distancia_entre_sensores(distancias, p1->idSensor, p2->idSensor);
            time_t delta = difftime(p2->data, p1->data);
            if (delta <= 0 || dist <= 0)
            {
                current = current->next;
                continue;
            }

            float horas = delta / 3600.0f;
            float vel = dist / horas;

            // Encontrar ou criar entrada
            int idx = -1;
            for (int i = 0; i < count; i++)
            {
                if (ranking[i].veiculo->codVeiculo == v->codVeiculo)
                {
                    idx = i;
                    break;
                }
            }

            if (idx == -1)
            {
                if (count >= cap)
                {
                    cap = (cap == 0) ? 8 : cap * 2;
                    VelocidadeVeiculoItem *temp = realloc(ranking, cap * sizeof(VelocidadeVeiculoItem));
                    if (!temp)
                    {
                        perror("Erro a alocar memória");
                        free(ranking);
                        return;
                    }
                    ranking = temp;
                }

                ranking[count].veiculo = v;
                ranking[count].soma_velocidade = 0.0f;
                ranking[count].count = 0;
                ranking[count].media = 0.0f;
                idx = count++;
            }

            ranking[idx].soma_velocidade += vel;
            ranking[idx].count++;

            current = current->next->next;
        }
        else
        {
            current = current->next;
        }
    }

    // Calcular média e encontrar veículo mais rápido
    float max_media = 0.0f;
    int idx_max = -1;

    for (int i = 0; i < count; i++)
    {
        if (ranking[i].count > 0)
        {
            ranking[i].media = ranking[i].soma_velocidade / ranking[i].count;
            if (ranking[i].media > max_media)
            {
                max_media = ranking[i].media;
                idx_max = i;
            }
        }
    }

    if (idx_max >= 0)
    {
        Veiculo *v = ranking[idx_max].veiculo;
        Dono *d = v->dono;

        printf("\n=== Carro Mais Rápido ===\n");
        printf("Matrícula : %s\n", v->matricula);
        printf("Marca     : %s\n", v->marca);
        printf("Modelo    : %s\n", v->modelo);
        printf("Velocidade Média: %.2f km/h\n", ranking[idx_max].media);

        if (d)
        {
            printf("\n--- Dono ---\n");
            printf("Nome: %s\n", d->nome);
            printf("NIF : %d\n", d->numContribuinte);
        }
    }
    else
    {
        printf("Nenhuma velocidade válida foi encontrada.\n");
    }

    free(ranking);
}

/**
 * \brief Exporta as passagens para um ficheiro CSV.
 * \param list Lista de passagens.
 * \param filename Nome do ficheiro CSV.
 */
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
        current = current->next;
    }

    fclose(file);
}