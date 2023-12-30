#ifndef SOCKET_CLIENT_H
#define SOCKET_CLIENT_H

#define UNIXSTR_PATH "/tmp/s.projeto2str"
#define UNIXDG_PATH "/tmp/s.projeto2dg"
#define UNIXDG_TMP "/tmp/dg2080021"

extern int sock_fd;

void openSocket();
void closeSocket();

#endif
