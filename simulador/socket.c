#include "socket.h"
#include <sys/un.h>
#include <sys/socket.h>

int

void createSocket()
{
    int sockfd, newsockfd, clilen, childpid, servlen;
    struct sockaddr_un cli_addr, serv_addr;

    if ((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
        err_dump("server: can't open stream socket");

    bzero((char *)&serv_addr, sizeof(serv_addr)); // limpeza preventiva
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, UNIXSTR_PATH); // ficheiro para clientes identificarem servidor

    servlen = strlen(serv_addr.sun_path) + sizeof(serv_addr.sun_family);
    unlink(UNIXSTR_PATH);
    if (bind(sockfd, (struct sockaddr *)&serv_addr, servlen) < 0)
        err_dump("server, can't bind local address");

    listen(sockfd, 1); // aceita 1 cliente de uma só vez

    for (;;)
    {
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr,
                           &clilen);
        if (newsockfd < 0)
            err_dump("server: accept error");

        if ((childpid = fork()) < 0) // processo filho lida com o cliente
            err_dump("server: fork error");

        else if (childpid == 0)
        {
            /* Processo filho que vai atender o cliente.
               Fechar sockfd é sanitário, já que não é
               utilizado pelo processo filho.
               Os dados recebidos do cliente são reenviados
               para o cliente */

            close(sockfd);
            str_echo(newsockfd);
            exit(0);
        }

        /* Processo pai.
           Fechar newsockfd é sanitário, já que não é
           utilizado pelo processo pai */

        close(newsockfd);
    }
}

void closeSocket()
{

}
