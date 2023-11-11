struct Espaco
{
    const char *nome;
    int lotacao;
    int minimo;
    const char *cargo;
};

struct Config
{
    int tempoMedioChegada;
    int probDesistencia;
    int tempoSimulacao;
    int numeroEspacos;
    struct Espaco *espacos;
};
