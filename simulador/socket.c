#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "socket.h"
#include "util.h"

void openSocket()
{
    int clilen, childpid, servlen;
    struct sockaddr_un cli_addr, serv_addr;

    if ((sock_fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
        printf("server: can't open stream socket\n");

    bzero((char *)&serv_addr, sizeof(serv_addr)); // limpeza preventiva
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, UNIXSTR_PATH); // ficheiro para clientes identificarem servidor

    servlen = strlen(serv_addr.sun_path) + sizeof(serv_addr.sun_family);
    unlink(UNIXSTR_PATH);

    if (bind(sock_fd, (struct sockaddr *)&serv_addr, servlen) < 0)
        printf("server, can't bind local address\n");

    listen(sock_fd, 1); // aceita 1 cliente

    clilen = sizeof(cli_addr);

    printf("Aguardando conecxão do monitor...\n");
    sock_monitor_fd = accept(sock_fd, (struct sockaddr *)&cli_addr, &clilen);

    if (sock_monitor_fd < 0)
    {
        printf("server: accept error\n");
    }
    printf("Monitor conectado, simulação iniciada.\n");
}

void closeSocket()
{
    printf("Terminando conecxão com monitor.\n");
    close(sock_monitor_fd);
    close(sock_fd);
}
