#include "includes.h"

t_history *history_tail(t_history *h)
{
    if (!h)
        return NULL;
    while (h->next)
        h = h->next;
    return h;
}

char *reader(t_history **history)
{
    t_history *cursor = NULL;   // NULL = empty prompt
    char buffer[4096];
    int len = 0;
    char c;

    buffer[0] = 0;

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
                buffer[len] = 0;
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

            // recompute tail every time (important)
            t_history *tail = history_tail(*history);

            // UP ARROW
            if (seq[0] == '[' && seq[1] == 'A')
            {
                if (!tail)
                    continue;

                if (cursor == NULL)
                    cursor = tail;          // first UP -> last command
                else if (cursor->prev)
                    cursor = cursor->prev; // go back in history

                write(1, "\r\033[K", 4);
                write(1, "ashell> ", 8);

                strncpy(buffer, cursor->cmd, sizeof(buffer) - 1);
                buffer[sizeof(buffer) - 1] = 0;
                len = (int)strlen(buffer);
                write(1, buffer, len);
            }
            // DOWN ARROW
            else if (seq[0] == '[' && seq[1] == 'B')
            {
                if (cursor && cursor->next)
                    cursor = cursor->next;
                else
                    cursor = NULL; // back to empty prompt

                write(1, "\r\033[K", 4);
                write(1, "ashell> ", 8);

                if (cursor)
                {
                    strncpy(buffer, cursor->cmd, sizeof(buffer) - 1);
                    buffer[sizeof(buffer) - 1] = 0;
                    len = (int)strlen(buffer);
                    write(1, buffer, len);
                }
                else
                {
                    len = 0;
                    buffer[0] = 0;
                }
            }

            continue;
        }

        // normal character
        if (len < (int)sizeof(buffer) - 1)
        {
            buffer[len++] = c;
            buffer[len] = 0;
            write(1, &c, 1);
            cursor = NULL; // typing resets history navigation
        }
    }

    return strdup(buffer);
}




