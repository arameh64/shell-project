#include "includes.h"
\
char *reader(t_history **history)
{
    static t_history *hist_cursor = NULL;
    char buffer[4096];
    int len = 0;
    char c;

    hist_cursor = *history;

    write(1, "ashell> ", 8);

    while (1)
    {
        if (read(STDIN_FILENO, &c, 1) != 1)
            return NULL;

        // ENTER
        if (c == '\n')
        {
            write(1, "\n", 1);
            break;
        }

        // BACKSPACE
        if (c == 127 || c == 8)
        {
            if (len > 0)
            {
                len--;
                write(1, "\b \b", 3);
            }
            continue;
        }

        // ESC (arrow keys)
        if (c == 27)
        {
            char seq[2];
            if (read(STDIN_FILENO, &seq[0], 1) != 1) continue;
            if (read(STDIN_FILENO, &seq[1], 1) != 1) continue;

            if (seq[0] == '[' && seq[1] == 'A') // UP
            {
                if (hist_cursor && hist_cursor->next)
                    hist_cursor = hist_cursor->next;

                write(1, "\r\033[K", 4);
                write(1, "ashell> ", 8);

                if (hist_cursor)
                {
                    strcpy(buffer, hist_cursor->cmd);
                    len = strlen(buffer);
                    write(1, buffer, len);
                }
            }
            else if (seq[0] == '[' && seq[1] == 'B') // DOWN
            {
                if (hist_cursor && hist_cursor->prev)
                    hist_cursor = hist_cursor->prev;

                write(1, "\r\033[K", 4);
                write(1, "ashell> ", 8);

                if (hist_cursor)
                {
                    strcpy(buffer, hist_cursor->cmd);
                    len = strlen(buffer);
                    write(1, buffer, len);
                }
            }

            continue;
        }

        // normal character
        buffer[len++] = c;
        write(1, &c, 1);
    }

    buffer[len] = '\0';
    return strdup(buffer);
}