#ifndef INCLUDES_H
#define INCLUDES_H

#define _GNU_SOURCE

// ===== SYSTEM INCLUDES =====
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>

// ===== GLOBALS =====
extern char **environ;

// ===== STRUCTS & ENUMS =====

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


// ===== HISTORY =====
void add_history(t_history **head, char *line);
void print_history(t_history *history);

// ===== READER =====
char *reader();


// ===== LEXER / TOKENIZER =====
t_token *new_token(t_token_type type, char *value);
void    add_token(t_token **head, t_token *newt);
t_token *tokenize(char *cmd);
void    print_tokens(t_token *t);
void    free_tokens(t_token *t);


// ===== PARSER =====
t_cmd   *new_cmd(void);
void    add_cmd(t_cmd **head, t_cmd *newc);
void    add_arg(t_cmd *cmd, char *word);
t_cmd   *parse_tokens(t_token *tokens);
void    free_cmds(t_cmd *cmds);


// ===== EXECUTOR =====
void execute_commands(t_cmd *cmds, t_history **history);
void execute_single(t_cmd *cmd, t_history **history);
void execute_pipeline(t_cmd *cmds, t_history **history);
void handle_redirections(t_cmd *cmd);
int externals(char **argv);


// ===== BUILTINS =====
int  is_builtin(char *cmd);
void run_builtin(t_cmd *cmd, t_history **history);

int builtin_cd(char **argv);
int builtin_echo(char **argv);
int builtin_pwd(void);
int builtin_exit(t_history **history);
int builtin_env(void);
int builtin_export(char **argv);
int builtin_unset(char **argv);


// ===== EXTERNALS =====
int externals(char **argv);

// ===== UTILS =====
void free_cmds(t_cmd *cmds);
void print_tokens(t_token *t);
void free_tokens(t_token *t);

#endif
