#include <stdio.h>

extern int fd;
extern char *ptr;
extern int nbytes;
extern int readn(int fd, char *ptr, int nbytes);
extern int writen(int fd, char *ptr, int nbytes);
extern int readline(int fd, char *ptr, int maxlen);
extern void err_dump(char *msg);
extern void str_cli(FILE *fp, int sockfd);
extern void str_echo(int sockfd);
