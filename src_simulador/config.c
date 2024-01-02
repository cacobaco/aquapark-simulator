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
        return;
    }

    cJSON *tempoSimulacao = cJSON_GetObjectItemCaseSensitive(json, "tempo_simulacao");
    if (!cJSON_IsNumber(tempoSimulacao))
    {
        printf("Ocorreu um erro ao carregar tempo_simulacao: não está definido ou não é número\n");
        exit(1);
        return;
    }

    cJSON *tempoMedioChegada = cJSON_GetObjectItemCaseSensitive(json, "tempo_medio_chegada");
    if (!cJSON_IsNumber(tempoMedioChegada))
    {
        printf("Ocorreu um erro ao carregar tempo_medio_chegada: não está definido ou não é número\n");
        exit(1);
        return;
    }

    cJSON *probEntradaEspaco = cJSON_GetObjectItemCaseSensitive(json, "prob_entrada_espaco");
    if (!cJSON_IsNumber(probEntradaEspaco))
    {
        printf("Ocorreu um erro ao carregar prob_entrada_espaco: não está definido ou não é número\n");
        exit(1);
        return;
    }

    cJSON *probSaidaEspaco = cJSON_GetObjectItemCaseSensitive(json, "prob_saida_espaco");
    if (!cJSON_IsNumber(probSaidaEspaco))
    {
        printf("Ocorreu um erro ao carregar prob_saida_espaco: não está definido ou não é número\n");
        exit(1);
        return;
    }

    cJSON *probSaidaParque = cJSON_GetObjectItemCaseSensitive(json, "prob_saida_parque");
    if (!cJSON_IsNumber(probSaidaParque))
    {
        printf("Ocorreu um erro ao carregar prob_saida_parque: não está definido ou não é número\n");
        exit(1);
        return;
    }

    cJSON *lotacaoMaxima = cJSON_GetObjectItemCaseSensitive(json, "lotacao_maxima");
    if (!cJSON_IsNumber(lotacaoMaxima))
    {
        printf("Ocorreu um erro ao carregar lotacao_maxima: não está definido ou não é número\n");
        exit(1);
        return;
    }

    cJSON *probVip = cJSON_GetObjectItemCaseSensitive(json, "prob_vip");
    if (!cJSON_IsNumber(probVip))
    {
        printf("Ocorreu um erro ao carregar prob_vip: não está definido ou não é número\n");
        exit(1);
        return;
    }

    cJSON *espacosJson = cJSON_GetObjectItemCaseSensitive(json, "espacos");
    if (!cJSON_IsArray(espacosJson))
    {
        printf("Ocorreu um erro ao carregar os espaços da configuração: não está definido ou não é número\n");
        exit(1);
        return;
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
            printf("Ocorreu um erro ao carregar o nome de um dos espaços da configuração: não está definido ou não é string\n");
            exit(1);
            return;
        }
        espacos[i].nome = nome->valuestring;

        cJSON *lotacaoMaxima = cJSON_GetObjectItemCaseSensitive(espacoJson, "lotacao_maxima");
        if (!cJSON_IsNumber(lotacaoMaxima))
        {
            printf("Ocorreu um erro ao carregar a lotação máxima do espaço %s: não está definido ou não é número\n", espacos[i].nome);
            exit(1);
            return;
        }
        espacos[i].lotacaoMaxima = lotacaoMaxima->valueint;
        espacos[i].lotacao = 0;
        pthread_mutex_init(&(espacos[i].mutexLotacao), NULL);

        cJSON *lotacaoMaximaFila = cJSON_GetObjectItemCaseSensitive(espacoJson, "lotacao_maxima_fila");
        if (cJSON_IsNumber(lotacaoMaximaFila))
        {
            espacos[i].bFila = 1;
            espacos[i].lotacaoMaximaFila = lotacaoMaximaFila->valueint;
            espacos[i].lotacaoFila = 0;
            espacos[i].numVips = 0;
            pthread_mutex_init(&(espacos[i].mutexLotacaoFila), NULL);
            sem_init(&(espacos[i].semaforoFila), 0, 0);
            sem_init(&(espacos[i].semaforoVips), 0, 0);
        }
        else
        {
            espacos[i].bFila = 0;
        }

        cJSON *duracao = cJSON_GetObjectItemCaseSensitive(espacoJson, "duracao");
        if (cJSON_IsNumber(duracao))
        {
            espacos[i].bDuracao = 1;
            espacos[i].duracao = duracao->valueint;
        }
        else
        {
            espacos[i].bDuracao = 0;
        }

        cJSON *corrida = cJSON_GetObjectItemCaseSensitive(espacoJson, "corrida");
        if (cJSON_IsBool(corrida))
        {
            if (!espacos[i].bDuracao)
            {
                printf("Ocorreu um erro ao carregar corrida do espaço %s: duração não definida\n", espacos[i].nome);
                exit(1);
                return;
            }

            espacos[i].bCorrida = corrida->valueint;
            pthread_mutex_init(&(espacos[i].mutexCorrida), NULL);
            sem_init(&(espacos[i].semaforoCorrida), 0, 0);
            sem_init(&(espacos[i].semaforoCorridaSaida), 0, espacos[i].lotacaoMaxima);
            espacos[i].corredores = malloc(espacos[i].lotacaoMaxima * sizeof(pthread_t));
        }
        else
        {
            espacos[i].bCorrida = 0;
        }

        i++;
    }

    config = malloc(sizeof(Config));
    config->tempoSimulacao = tempoSimulacao->valueint;
    config->tempoMedioChegada = tempoMedioChegada->valueint;
    config->probEntradaEspaco = probEntradaEspaco->valuedouble;
    config->probSaidaEspaco = probSaidaEspaco->valuedouble;
    config->probSaidaParque = probSaidaParque->valuedouble;
    config->lotacaoMaxima = lotacaoMaxima->valueint;
    config->probVip = probVip->valuedouble;
    config->utilizadores = malloc(config->lotacaoMaxima * sizeof(pthread_t));

    for (int i = 0; i < config->lotacaoMaxima; i++)
    { // limpeza preventiva
        config->utilizadores[i] = 0;
    }

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
    printf("Probabilidade de utilizador ser vip: %f\n", config->probVip);
    printf("Número de espaços: %i\n", config->numeroEspacos);

    for (int i = 0; i < config->numeroEspacos; i++)
    {
        Espaco *espaco = &(config->espacos[i]);

        printf("Espaço %i:\n", i + 1);
        printf("\tNome: %s\n", espaco->nome);
        printf("\tLotação máxima: %i\n", espaco->lotacaoMaxima);

        if (espaco->bFila)
        {
            printf("\tLotação máxima da fila: %i\n", espaco->lotacaoMaximaFila);
        }

        if (espaco->bDuracao)
        {
            printf("\tDuração: %i\n", espaco->duracao);
        }

        if (espaco->bCorrida)
        {
            printf("\tCorrida: Sim\n");
        }
        else
        {
            printf("\tCorrida: Não\n");
        }
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
        pthread_mutex_destroy(&(espaco->mutexLotacao));

        if (espaco->bFila)
        {
            pthread_mutex_destroy(&(espaco->mutexLotacaoFila));
            sem_destroy(&(espaco->semaforoFila));
            sem_destroy(&(espaco->semaforoVips));
        }

        if (espaco->bCorrida)
        {
            pthread_mutex_destroy(&(espaco->mutexCorrida));
            sem_destroy(&(espaco->semaforoCorrida));
            sem_destroy(&(espaco->semaforoCorridaSaida));
            free(espaco->corredores);
        }
    }

    free(config->espacos);
    free(config);
}
