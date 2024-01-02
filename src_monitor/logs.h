#ifndef LOGS_H
#define LOGS_H

#define LOG_FILE "logs.txt"

extern FILE *logFile;

void openLogFile();
void closeLogFile();
void addLogMessage(char *message);

#endif
