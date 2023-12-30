#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "config.h"
#include "structs.h"
#include "cJSON.h"

Config *config;

/*
 * função que carrega a configuração do ficheiro de configuração e cria a
 * estrutura de dados correspondente na variável global config
 */
void loadConfig()
{
    FILE *f = fopen(CONFIG_FILE, "r");

    if (f == NULL)
    {
        printf("%s", "Ocorreu um erro ao abrir o ficheiro de configuração.\n");
        exit(1);
    }

    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *fcontent = malloc(fsize + 1);
    fread(fcontent, fsize, 1, f);

    fclose(f);

    cJSON *json = cJSON_Parse(fcontent);
    if (json == NULL)
    {
        printf("Ocorreu um erro ao carregar a configuração.\n");
        exit(1);
    }

    cJSON *tempoSimulacao = cJSON_GetObjectItemCaseSensitive(json, "tempo_simulacao");
    if (!cJSON_IsNumber(tempoSimulacao))
    {
        printf("Ocorreu um erro ao carregar tempo_simulacao.\n");
        exit(1);
    }

    cJSON *tempoMedioChegada = cJSON_GetObjectItemCaseSensitive(json, "tempo_medio_chegada");
    if (!cJSON_IsNumber(tempoMedioChegada))
    {
        printf("Ocorreu um erro ao carregar tempo_medio_chegada.\n");
        exit(1);
    }

    cJSON *probEntradaEspaco = cJSON_GetObjectItemCaseSensitive(json, "prob_entrada_espaco");
    if (!cJSON_IsNumber(probEntradaEspaco))
    {
        printf("Ocorreu um erro ao carregar prob_entrada_espaco.\n");
        exit(1);
    }

    cJSON *probSaidaEspaco = cJSON_GetObjectItemCaseSensitive(json, "prob_saida_espaco");
    if (!cJSON_IsNumber(probSaidaEspaco))
    {
        printf("Ocorreu um erro ao carregar prob_saida_espaco.\n");
        exit(1);
    }

    cJSON *probSaidaParque = cJSON_GetObjectItemCaseSensitive(json, "prob_saida_parque");
    if (!cJSON_IsNumber(probSaidaParque))
    {
        printf("Ocorreu um erro ao carregar prob_saida_parque.\n");
        exit(1);
    }

    cJSON *lotacaoMaxima = cJSON_GetObjectItemCaseSensitive(json, "lotacao_maxima");
    if (!cJSON_IsNumber(lotacaoMaxima))
    {
        printf("Ocorreu um erro ao carregar lotacao_maxima.\n");
        exit(1);
    }

    cJSON *espacosJson = cJSON_GetObjectItemCaseSensitive(json, "espacos");
    if (!cJSON_IsArray(espacosJson))
    {
        printf("Ocorreu um erro ao carregar os espaços da configuração.\n");
        exit(1);
    }

    int numeroEspacos = cJSON_GetArraySize(espacosJson);
    Espaco *espacos = malloc(numeroEspacos * sizeof(Espaco));

    int i = 0;
    cJSON *espacoJson = NULL;
    cJSON_ArrayForEach(espacoJson, espacosJson)
    {
        cJSON *nome = cJSON_GetObjectItemCaseSensitive(espacoJson, "nome");
        if (!cJSON_IsString(nome) || nome->valuestring == NULL)
        {
            printf("Ocorreu um erro ao carregar o nome de um dos espaços da configuração.\n");
            exit(1);
        }
        espacos[i].nome = nome->valuestring;

        cJSON *lotacaoMaxima = cJSON_GetObjectItemCaseSensitive(espacoJson, "lotacao_maxima");
        if (cJSON_IsNumber(lotacaoMaxima))
        {
            espacos[i].lotacaoMaxima = lotacaoMaxima->valueint;
        }

        espacos[i].numUtilizadores = 0;

        // cJSON *cargo = cJSON_GetObjectItemCaseSensitive(espacoJson, "cargo");
        // if (cJSON_IsString(cargo) && cargo->valuestring != NULL)
        // {
        //     espacos[i].cargo = cargo->valuestring;
        // }
        // else
        // {
        //     espacos[i].cargo = NULL;
        // }

        i++;
    }

    config = malloc(sizeof(Config));
    config->tempoSimulacao = tempoSimulacao->valueint;
    config->tempoMedioChegada = tempoMedioChegada->valueint;
    config->probEntradaEspaco = probEntradaEspaco->valuedouble;
    config->probSaidaEspaco = probSaidaEspaco->valuedouble;
    config->probSaidaParque = probSaidaParque->valuedouble;
    config->lotacaoMaxima = lotacaoMaxima->valueint;
    config->utilizadores = malloc(config->lotacaoMaxima * sizeof(pthread_t));
    config->numeroEspacos = numeroEspacos;
    config->espacos = espacos;
}

/*
 * função que imprime a configuração
 */
void printConfig()
{
    printf("Tempo de simulação: %i\n", config->tempoSimulacao);
    printf("Tempo médio de chegada: %i\n", config->tempoMedioChegada);
    printf("Probabilidade de entrada no espaço: %f\n", config->probEntradaEspaco);
    printf("Probabilidade de saída do espaço: %f\n", config->probSaidaEspaco);
    printf("Probabilidade de saída do parque: %f\n", config->probSaidaParque);
    printf("Lotação máxima do parque: %i\n", config->lotacaoMaxima);
    printf("Número de espaços: %i\n", config->numeroEspacos);

    for (int i = 0; i < config->numeroEspacos; i++)
    {
        Espaco *espaco = &(config->espacos[i]);

        printf("Espaço %i:\n", i + 1);
        printf("\tNome: %s\n", espaco->nome);
        printf("\tLotação máxima: %i\n", espaco->lotacaoMaxima);
        // printf("\tCargo necessário: %s\n", espaco->cargo);
    }
}

/*
 * função que liberta a memória alocada para a configuração
 */
void freeConfig()
{
    free(config->utilizadores);

    for (int i = 0; i < config->numeroEspacos; i++)
    {
        Espaco *espaco = &(config->espacos[i]);

        free(espaco->nome);
        // free(espaco->cargo);
    }

    free(config->espacos);
    free(config);
}
