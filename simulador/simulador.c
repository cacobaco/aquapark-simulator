#include "config.h"
#include "socket.h"
#include "utilizador.h"

int main(int argc, char const *argv[])
{
    // loadConfig();
    // printConfig();
    // freeMemory();

    loadConfig();
    // printConfig();
    openSocket();

    while (1) // jony
    {
        entraParque();
    }

    closeSocket();
    freeConfig();
    return 0;
}
