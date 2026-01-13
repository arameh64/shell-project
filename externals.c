#include "includes.h"

int externals(char **argv)
{
    execvp(argv[0], argv);
    perror(argv[0]);
    return 1;
}