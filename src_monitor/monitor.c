#include <stdio.h>
#include <stdlib.h>
#include "socket_server.h"
#include "logs.h"

void openMenu();

void main(int argc, char const *argv[])
{
    openMenu();
    exit(0);
}

void openMenu()
{
    int option = -1;

    while (option != 0)
    {
        printf("-------- Menu --------\n");
        printf("1 - Iniciar simulação\n");
        printf("0 - Fechar aplicação\n");
        printf("Escolha uma opção: ");

        scanf("%d", &option);

        printf("\n");

        switch (option)
        {
        case 0:
            printf("A fechar aplicação...\n");
            exit(0);
            break;
        case 1:
            openLogFile();
            openSocket();
            closeSocket();
            closeLogFile();
            break;
        default:
            printf("Opção inválida.\n");
            break;
        }

        printf("\n");
    }
}
