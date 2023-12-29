#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "socket_server.h"
#include "util.h"

#define MAX_LINE 512

int sock_fd, sock_simulador_fd;

void openSocket()
{
    int clilen, childpid, servlen;
    struct sockaddr_un cli_addr, serv_addr;

    if ((sock_fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
    {
        printf("server: can't open stream socket\n");
        return;
    }

    bzero((char *)&serv_addr, sizeof(serv_addr)); // limpeza preventiva
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, UNIXSTR_PATH); // ficheiro para clientes identificarem servidor

    servlen = strlen(serv_addr.sun_path) + sizeof(serv_addr.sun_family);
    unlink(UNIXSTR_PATH);

    if (bind(sock_fd, (struct sockaddr *)&serv_addr, servlen) < 0)
    {
        printf("server, can't bind local address\n");
        return;
    }

    listen(sock_fd, 1); // aceita 1 cliente

    clilen = sizeof(cli_addr);

    execlp("./simulador", "simulador", NULL);
    printf("Aguardando conecxão do simulador...\n");
    sock_simulador_fd = accept(sock_fd, (struct sockaddr *)&cli_addr, &clilen);

    if (sock_simulador_fd < 0)
    {
        printf("server: accept error\n");
        return;
    }

    while (1)
    {
        char buf[MAX_LINE];
        int n = readline(sock_fd, buf, MAX_LINE);

        if (n < 0)
            printf("str_cli: readline error\n");

        buf[n - 1] = '\n';
        buf[n] = 0;

        fputs(buf, stdout);
    }

    printf("Simulador conectado, simulação iniciada.\n");
}

void closeSocket()
{
    printf("Simulação terminada, fechando conecxão...\n");
    close(sock_simulador_fd);
    close(sock_fd);
    printf("Conecxão fechada.\n");
}
