#include "includes.h"
 
//      config file path    =      ~/.ashrc

void load_rc(t_history ** history)
{
    printf("DEBUG: loading rc file\n");
    
    char path[PATH_MAX];
    snprintf(path, sizeof(path), "%s/.ashrc", getenv("HOME"));
    printf("DEBUG: rc path = %s\n", path);
    
    FILE *f = fopen(path, "r");
    if (!f)
        return;

    char *line = NULL;
    size_t len = 0;

    while (getline(&line, &len, f) != -1)
    {
        if (line[0] == '\n' || line[0] == '#')
            continue;

        process_line(line, history); // same pipeline as main loop
    }

    free(line);
    fclose(f);
}

void process_line(char *raw, t_history **history)
{
    t_token *toks;
    t_cmd   *cmds;

    if (*raw)
        add_history(history, raw);

    toks = tokenize(raw);
    if (!toks)
        return;

    cmds = parse_tokens(toks);
    if (!cmds)
    {
        free_tokens(toks);
        return;
    }

    execute_commands(cmds, history);

    free_cmds(cmds);
    free_tokens(toks);
}






