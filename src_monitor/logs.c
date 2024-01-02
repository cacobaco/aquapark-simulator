#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "logs.h"

FILE *logFile;

char *getDate();
char *getTime();

void openLogFile()
{
    logFile = fopen(LOG_FILE, "a");

    if (logFile == NULL)
    {
        printf("%s", "Ocorreu um erro ao abrir o ficheiro de logs.\n");
        exit(1);
    }
}

void closeLogFile()
{
    fclose(logFile);
}

void addLogMessage(char *message)
{
    fprintf(logFile, "[%s %s] %s", getDate(), getTime(), message);
}

char *getDate()
{
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    static char date[11];
    strftime(date, sizeof(date), "%d-%m-%Y", tm);
    return date;
}

char *getTime()
{
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    static char time[9];
    strftime(time, sizeof(time), "%H:%M:%S", tm);
    return time;
}
