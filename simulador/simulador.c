#include "config.h"

int main(int argc, char const *argv[])
{
    loadConfig();
    printConfig();
    freeMemory();
    return 0;
}
