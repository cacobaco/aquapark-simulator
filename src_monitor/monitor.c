#include <stdio.h>
#include <stdlib.h>
#include "socket_server.h"

#define LOG_FILE "report.txt"

// void gerarRelatorio();
// char *getReportDateTime();
// void catchSIGINT();

void openMenu();

void main(int argc, char const *argv[])
{
    openMenu();
    exit(0);
}

void openMenu()
{
    int option = -1;

    while (option != 0)
    {
        printf("-------- Menu --------\n");
        printf("1 - Iniciar simulação\n");
        printf("0 - Fechar aplicação\n");
        printf("Escolha uma opção: ");

        scanf("%d", &option);

        printf("\n");

        switch (option)
        {
        case 0:
            printf("A fechar aplicação...\n");
            exit(0);
            break;
        case 1:
            openSocket();
            break;
        default:
            printf("Opção inválida.\n");
            break;
        }

        printf("\n");
    }
}

// void gerarRelatorio()
// {
//     FILE *f = fopen(REPORT_FILE, "a");

//     if (f == NULL)
//     {
//         printf("%s", "Ocorreu um erro ao abrir o ficheiro de relatório.\n");
//         return 1;
//     }

//     fputs(getReportDateTime(), f);

//     printf("%s", "Introduza texto para o relatório.\n");

//     signal(SIGINT, catchSIGINT);

//     char line[100];

//     while (!stop && fgets(line, sizeof line, stdin))
//     {
//         if (strcmp(line, "\n"))
//         {
//             fputs(line, f);
//         }
//     }

//     fputs(getReportDateTime(), f);
//     fclose(f);
// }

// /* Returns current date and time for report. */
// char *getReportDateTime()
// {
//     time_t t = time(NULL);

//     if (t == -1)
//     {
//         return "[error]\n";
//     }

//     return ctime(&t);
// }

// void catchSIGINT()
// {
//     stop = 1;
// }
