#include <stdio.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include "socket.h"

#define REPORT_FILE "report.txt"

int stop = 0;

void gerarRelatorio();
char *getReportDateTime();
void catchSIGINT();

int main(int argc, char const *argv[])
{
    openSocket();
    closeSocket();
    return 0;
}

void gerarRelatorio()
{
    FILE *f = fopen(REPORT_FILE, "a");

    if (f == NULL)
    {
        printf("%s", "Ocorreu um erro ao abrir o ficheiro de relatório.\n");
        return 1;
    }

    fputs(getReportDateTime(), f);

    printf("%s", "Introduza texto para o relatório.\n");

    signal(SIGINT, catchSIGINT);

    char line[100];

    while (!stop && fgets(line, sizeof line, stdin))
    {
        if (strcmp(line, "\n"))
        {
            fputs(line, f);
        }
    }

    fputs(getReportDateTime(), f);
    fclose(f);
}

/* Returns current date and time for report. */
char *getReportDateTime()
{
    time_t t = time(NULL);

    if (t == -1)
    {
        return "[error]\n";
    }

    return ctime(&t);
}

void catchSIGINT()
{
    stop = 1;
}
