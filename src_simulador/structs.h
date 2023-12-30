#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct
{
    char *nome;
    int lotacaoMaxima;
    // const char *cargo;
} Espaco;

typedef struct
{
    int tempoSimulacao;
    int tempoMedioChegada;
    double probDesistencia;
    double probSaida;
    int numeroEspacos;
    Espaco *espacos;
} Config;

typedef struct // jony
{
    int id;
    // const char *cargo;
} Utilizador;

#endif
