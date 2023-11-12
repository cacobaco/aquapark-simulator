#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "socket.h"
#include "util.h"

#define MAX_LINE 512

void openSocket()
{
    int sock_fd, servlen;
    struct sockaddr_un serv_addr;

    if ((sock_fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
        printf("client: can't open stream socket\n");

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, UNIXSTR_PATH);
    servlen = strlen(serv_addr.sun_path) + sizeof(serv_addr.sun_family);

    if (connect(sock_fd, (struct sockaddr *)&serv_addr, servlen) < 0)
        printf("client: can't connect to server\n");
    else
    {
        while (1)
        {
            char buf[MAX_LINE];
            int n = readline(sock_fd, buf, MAX_LINE);

            if (n < 0)
                printf("str_cli:readline error\n");

            buf[n - 1] = '\n';
            buf[n] = 0;

            fputs(buf, stdout);
        }
    }
}

void closeSocket()
{
    printf("Terminando conecxão com simulador.\n");
    close(sock_fd);
}
