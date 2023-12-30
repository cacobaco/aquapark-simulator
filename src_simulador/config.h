#ifndef CONFIG_H
#define CONFIG_H

#include "structs.h"

#define CONFIG_FILE "config.json"

extern Config *config;

void loadConfig();
void printConfig();
void freeConfig();

#endif
