#include "includes.h"

int main(void)
{
    char        *raw;
    t_token     *toks;
    t_cmd       *cmds;
    t_history   *history = NULL;

    load_rc(&history);   // runs config file
    
    while (1)
    {
        raw = reader();
        if (!raw)
            break;

        if (*raw)
            add_history(&history, raw);

        toks = tokenize(raw);
        if (!toks)
        {
            free(raw);
            continue;
        }

        cmds = parse_tokens(toks);
        if (!cmds)
        {
            free_tokens(toks);
            free(raw);
            continue;
        }

        expand_cmd_vars(cmds);
        
        execute_commands(cmds, &history);

        free_cmds(cmds);
        free_tokens(toks);
        free(raw);
    }

    return 0;
}
