#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "socket_client.h"
#include "util.h"

int sock_fd;

void openSocket()
{
    int servlen;
    struct sockaddr_un serv_addr;

    if ((sock_fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
    {
        // printf("Erro ao abrir socket.\n");
        exit(1);
        return;
    }

    // printf("Socket aberta.\n");

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, UNIXSTR_PATH);
    servlen = strlen(serv_addr.sun_path) + sizeof(serv_addr.sun_family);

    // printf("Conectando ao monitor...\n");

    if (connect(sock_fd, (struct sockaddr *)&serv_addr, servlen) < 0)
    {
        // printf("Erro ao conectar ao monitor.\n");
        exit(1);
        return;
    }

    // printf("Conectado ao monitor.\n");
}

void closeSocket()
{
    // printf("Fechando socket...\n");
    close(sock_fd);
    // printf("Socket fechada.\n");
}
