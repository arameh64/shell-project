#include "includes.h"

char *reader()
{
    char *line = NULL;
    size_t len = 0;

    ssize_t nread = getline(&line, &len, stdin);
    if (nread == -1)
    {
        free(line);
        return NULL;
    }

    line[strcspn(line, "\n")] = 0;
    return line;
}
