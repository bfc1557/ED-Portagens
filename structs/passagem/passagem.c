#include "passagem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

PassagemList criar_lista_passagens()
{
    return NULL;
}

void liberar_lista_passagens(PassagemList list)
{
    PassagemNode *cur = list;
    while (cur)
    {
        PassagemNode *tmp = cur;
        cur = cur->next;
        free(tmp);
    }
}

void inserir_passagem(PassagemList *list, Passagem passagem)
{
    PassagemNode *novo = malloc(sizeof *novo);
    if (!novo)
    {
        perror("malloc inserir_passagem");
        return;
    }
    novo->passagem = passagem;
    novo->next = *list;
    *list = novo;
}

static int parse_linha_passagem(const char *linha, Passagem *p)
{
    int dia, mes, ano, hh, mm, ss, ms;
    int lidos = sscanf(linha,
                       "%d\t%d\t%d-%d-%d %d:%d:%d.%d\t%d",
                       &p->idSensor,
                       &p->codVeiculo,
                       &dia, &mes, &ano,
                       &hh, &mm, &ss, &ms,
                       &p->tipoRegisto);

    if (lidos != 10)
        return -1;

    if (p->idSensor < 0 || p->codVeiculo < 0 || p->tipoRegisto < 0)
        return -2;

    struct tm t = {0};
    t.tm_mday = dia;
    t.tm_mon = mes - 1;
    t.tm_year = ano - 1900;
    t.tm_hour = hh;
    t.tm_min = mm;
    t.tm_sec = ss;

    time_t ts = mktime(&t);
    if (ts == (time_t)-1)
        return -3;

    p->data = ts;
    return 0;
}

void carregar_passagens(PassagemList *list, const char *filename, int batch_size)
{
    FILE *f = fopen(filename, "r");
    if (!f)
    {
        perror("carregar_passagens fopen");
        return;
    }

    char linha[256];
    long carregadas = 0;
    long ignoradas_nega = 0;
    long ignoradas_fmt = 0;
    long ignoradas_data = 0;

    while (fgets(linha, sizeof linha, f))
    {
        Passagem p;
        int rc = parse_linha_passagem(linha, &p);
        if (rc == 0)
        {
            inserir_passagem(list, p);
            if (++carregadas % batch_size == 0)
                printf("Carregadas %ld passagens...\n", carregadas);
        }
        else if (rc == -1)
        {
            ignoradas_fmt++;
        }
        else if (rc == -2)
        {
            ignoradas_nega++;
        }
        else if (rc == -3)
        {
            ignoradas_data++;
        }
    }

    fclose(f);

    printf("\n--- Resumo carregar_passagens ---\n");
    printf("Carregadas           : %ld\n", carregadas);
    printf("Ignoradas (negativas): %ld\n", ignoradas_nega);
    printf("Ignoradas (formato)  : %ld\n", ignoradas_fmt);
    printf("Ignoradas (data)     : %ld\n", ignoradas_data);
    printf("---------------------------------\n\n");
}

int calcular_memoria_passagens(PassagemList list)
{
    int total = 0;
    PassagemNode *current = list;
    while (current)
    {
        total += sizeof(PassagemNode);
        current = current->next;
    }
    return total;
}

void listar_passagens_periodo(PassagemList list, time_t inicio, time_t fim)
{
    PassagemNode *current = list;
    while (current)
    {
        if (current->passagem.data >= inicio && current->passagem.data <= fim)
        {
            char buffer[80];
            strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&current->passagem.data));
            printf("Sensor: %d, Veículo: %d, Data: %s, Tipo: %s\n",
                   current->passagem.idSensor, current->passagem.codVeiculo,
                   buffer, current->passagem.tipoRegisto ? "Saída" : "Entrada");
        }
        current = current->next;
    }
}

void listar_veiculos_periodo(PassagemList passagens, VeiculoHashTable *veiculos, time_t inicio, time_t fim)
{
    if (!passagens || !veiculos)
        return;

    // Primeiro, coletar todos os veículos únicos no período
    Veiculo **veiculos_periodo = NULL;
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
                // Verificar se já está na lista
                int encontrado = 0;
                for (int i = 0; i < count; i++)
                {
                    if (veiculos_periodo[i] == veiculo)
                    {
                        encontrado = 1;
                        break;
                    }
                }

                if (!encontrado)
                {
                    if (count >= capacity)
                    {
                        capacity = capacity == 0 ? 16 : capacity * 2;
                        Veiculo **temp = (Veiculo **)realloc(veiculos_periodo, capacity * sizeof(Veiculo *));
                        if (!temp)
                        {
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
    merge_sort((void **)veiculos_periodo, 0, count - 1, comparar_veiculos_matricula);

    // Imprimir resultados
    printf("\n=== Veículos no período selecionado ===\n");
    printf("%-10s %-15s %-15s %-4s %s\n", "Matrícula", "Marca", "Modelo", "Ano", "Dono");

    for (int i = 0; i < count; i++)
    {
        printf("%-10s %-15s %-15s %-4d %s\n",
               veiculos_periodo[i]->matricula,
               veiculos_periodo[i]->marca,
               veiculos_periodo[i]->modelo,
               veiculos_periodo[i]->ano,
               veiculos_periodo[i]->dono ? veiculos_periodo[i]->dono->nome : "N/A");
    }

    free(veiculos_periodo);
}

int registar_passagem(PassagemList *list, int idSensor, int codVeiculo, struct tm data_tm, int tipoRegisto)
{
    // Converter data para time_t
    time_t nova_data = mktime(&data_tm);
    if (nova_data == (time_t)-1)
    {
        fprintf(stderr, "Erro: data inválida (mktime falhou)\n");
        return -1;
    }

    for (PassagemNode *cur = *list; cur; cur = cur->next)
    {
        if (cur->passagem.codVeiculo == codVeiculo)
        {
            // Verificar se já existe passagem neste sensor
            if (cur->passagem.idSensor == idSensor)
            {
                fprintf(stderr,
                        "Erro: veículo %d já tem uma passagem no sensor %d\n",
                        codVeiculo, idSensor);
                return -3;
            }

            // Verificar se a nova data é anterior à última conhecida
            if (difftime(nova_data, cur->passagem.data) < 0)
            {
                fprintf(stderr,
                        "Erro: nova passagem é anterior à já existente para o veículo %d\n",
                        codVeiculo);
                return -2;
            }
        }
    }

    // Criar e inserir a nova passagem
    Passagem p;
    p.idSensor = idSensor;
    p.codVeiculo = codVeiculo;
    p.data = nova_data;
    p.tipoRegisto = tipoRegisto;

    inserir_passagem(list, p);
    return 0; // sucesso
}

void registar_passagem_interativo(PassagemList *list) {
    int idSensor, codVeiculo, tipoRegisto;
    char input[128];

    printf("\n=== Registar Nova Passagem ===\n");

    // ID do Sensor
    printf("ID do sensor: ");
    if (scanf("%d", &idSensor) != 1 || idSensor <= 0) {
        fprintf(stderr, "Erro: ID do sensor inválido.\n");
        while (getchar() != '\n'); // Limpar buffer
        return;
    }

    // Código do veículo
    printf("Código do veículo: ");
    if (scanf("%d", &codVeiculo) != 1 || codVeiculo <= 0) {
        fprintf(stderr, "Erro: código do veículo inválido.\n");
        while (getchar() != '\n');
        return;
    }

    // Data/hora
    printf("Data e hora (dd-mm-aaaa HH:MM:SS.mmm): ");
    getchar(); // consumir '\n' pendente
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Erro: leitura de data falhou.\n");
        return;
    }

    int d, m, y, h, min, s, ms;
    if (sscanf(input, "%d-%d-%d %d:%d:%d.%d", &d, &m, &y, &h, &min, &s, &ms) != 7) {
        fprintf(stderr, "Erro: formato da data inválido.\n");
        return;
    }

    struct tm data_tm = {0};
    data_tm.tm_mday = d;
    data_tm.tm_mon  = m - 1;
    data_tm.tm_year = y - 1900;
    data_tm.tm_hour = h;
    data_tm.tm_min  = min;
    data_tm.tm_sec  = s;

    // Tipo de registo
    printf("Tipo de registo (0/1): ");
    if (scanf("%d", &tipoRegisto) != 1 || (tipoRegisto != 0 && tipoRegisto != 1)) {
        fprintf(stderr, "Erro: tipo de registo inválido.\n");
        while (getchar() != '\n');
        return;
    }

    // Registar a passagem com validação
    int resultado = registar_passagem(list, idSensor, codVeiculo, data_tm, tipoRegisto);
    switch (resultado) {
        case 0:
            printf("Passagem registada com sucesso.\n");
            break;
        case -1:
            printf("Erro: data inválida.\n");
            break;
        case -2:
            printf("Erro: nova passagem é anterior à já existente para este veículo.\n");
            break;
        case -3:
            printf("Erro: o veículo já tem uma passagem neste sensor.\n");
            break;
        default:
            printf("Erro desconhecido.\n");
    }
}