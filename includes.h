#ifndef INCLUDES_H
#define INCLUDES_H

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

extern char **environ;


typedef struct s_history
{
    char *cmd;
    struct s_history *next;
} t_history;

typedef enum e_token_type
{
    TOK_WORD,
    TOK_PIPE,
    TOK_REDIR_IN,
    TOK_REDIR_OUT,
    TOK_REDIR_APPEND,
    TOK_BACKGROUND
} t_token_type;

typedef struct s_token
{
    t_token_type type;
    char *value;
    struct s_token *next;
} t_token;

typedef struct s_cmd
{
    char **argv;
    char *infile;
    char *outfile;
    int append;
    int background;
    struct s_cmd *next;
} t_cmd;

void add_history(t_history **head, char *line);

void execute_commands(t_cmd *cmds, t_history **history);

void free_cmds(t_cmd *cmds);


char    *reader();

t_token *new_token(t_token_type type, char *value);
void    add_token(t_token **head, t_token *newt);
t_token *tokenize(char *cmd);
void    print_tokens(t_token *t);
void    free_tokens(t_token *t);

t_cmd   *new_cmd();
void    add_cmd(t_cmd **head, t_cmd *newc);
void    add_arg(t_cmd *cmd, char *word);
t_cmd   *parse_tokens(t_token *tokens);


#endif
