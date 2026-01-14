#include "includes.h"
 
//      config file path    =      ~/.ashrc

void load_rc(t_history **history)
{
    char path[PATH_MAX];
    char *home = getenv("HOME");
    FILE *f;
    char *line = NULL;
    size_t len = 0;

    if (!home)
        return;

    snprintf(path, sizeof(path), "%s/.ashrc", home);

    f = fopen(path, "r");
    if (!f)
        return;

    while (getline(&line, &len, f) != -1)
    {
        // ignore empty/comment lines
        if (line[0] == '\n' || line[0] == '#')
            continue;

        process_line(line, history);
    }

    free(line);
    fclose(f);
}

void process_line(char *raw, t_history **history)
{
    t_token *toks;
    t_cmd   *cmds;

    if (!raw)
        return;

    // strip newline from getline() inputs (rc file + any other sources)
    raw[strcspn(raw, "\n")] = 0;

    // skip empty lines
    if (!*raw)
        return;

    // add to history safely (add_history does strdup)
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






