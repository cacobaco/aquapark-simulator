typedef struct
{
    const char *nome;
    int lotacao;
    int minimo;
    const char *cargo;
} Espaco;

typedef struct
{
    int tempoMedioChegada;
    int probDesistencia;
    int tempoSimulacao;
    int numeroEspacos;
    Espaco *espacos;
} Config;

typedef struct // jony
{
    int id;
    char *atracoesVisitadas[4];
    const char *cargo;
} Utilizador;
