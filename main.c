#include "includes.h"

int main()
{
    char *raw = reader();
    if (!raw)
        return 0;

    t_token *toks = tokenize(raw);
    print_tokens(toks);

    t_cmd *cmds = parse_tokens(toks);
    (void)cmds;

    free_tokens(toks);
    free(raw);
    return 0;
}
