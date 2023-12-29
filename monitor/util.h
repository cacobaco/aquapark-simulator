#ifndef UTIL_H
#define UTIL_H

extern int fd;
extern char *ptr;
extern int nbytes;
extern int readn(int fd, char *ptr, int nbytes);
extern int writen(int fd, char *ptr, int nbytes);
extern int readline(int fd, char *ptr, int maxlen);
extern void err_dump(char *msg);

#endif
