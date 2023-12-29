#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct
{
    const char *nome;
    int lotacaoMaxima;
    // const char *cargo;
} Espaco;

typedef struct
{
    int tempoSimulacao;
    int tempoMedioChegada;
    float probDesistencia;
    float probSaida;
    int numeroEspacos;
    Espaco *espacos;
} Config;

typedef struct // jony
{
    int id;
    // const char *cargo;
} Utilizador;

#endif
