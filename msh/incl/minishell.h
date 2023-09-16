/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inwagner <inwagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 21:09:26 by inwagner          #+#    #+#             */
/*   Updated: 2023/09/16 13:53:08 by inwagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libs/libft/incl/libft.h"

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <string.h>
# include <fcntl.h>
# include <dirent.h>
# include <curses.h>
# include <termios.h>
# include <termcap.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <readline/history.h>
# include <readline/readline.h>

# define OUT_OF_MEMORY 12
# define DEFAULT 0
# define ACTIVE 42
# define INACTIVE 43
# define CMD_NOT_EXECUTABLE 126
# define CMD_NOT_FOUND 127

# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

# ifndef FD_MAX
#  define FD_MAX 1024
# endif

typedef enum e_type
{
	APPEND = 1,
	HEREDOC,
	OVERWRITE,
	INPUT,
	PIPE,
	BUILTIN,
	EXEC,
	ARGUMENT
}	t_type;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

typedef struct s_token
{
	char			*str;
	t_type			type;
	struct s_token	*next;
	struct s_token	*prev;
}					t_token;

typedef struct s_here
{
	int				fd;
	struct s_here	*next;
}					t_here;

typedef struct s_cli
{
	char			**args;
	int				fd[2];
	int				hdoc;
	enum e_type		type;
	struct s_cli	*next;
}					t_cli;

typedef struct s_ctrl
{
	char			*input;
	t_token			*tokens;
	t_env			*env;
	t_cli			*commands;
	char			**pbox;
	int				status;
}					t_ctrl;

t_ctrl	*get_control(void);
void	set_signals(int mode);
void	prompt_user(const char *prompt);
int		tokenization(char *input);
char	*expand_token(char **str, int *j);
int		parser(void);

int		b_cd(char **path);
int		b_echo(char **args);
int		b_env(char **path, t_env *list);
int		b_exit(char **args);
int		b_export(t_env *env, char **args);
int		b_pwd(void);
int		b_unset(char **args, t_env *env);
int		export_without_args(t_env *env);
void	new_var(t_env *env, char *args);
void	call_builtin(t_cli *cli);

void	clear_tokens(t_token *token);
void	clear_pbox(char **pbox);
void	clear_cli(t_cli *cli);
void	exit_program(int code);

void	set_var(const char *src, t_env *node);
char	*get_var(char *str, int *i);
t_env	*add_var(t_env *prev, char *var);
t_env	*search_var(char *var);
t_env	*remove_var(char *str, t_env *list);
t_env	*parse_env(char **env);
void	update_env(char **argv, char *cmd, char *exec);
char	**stringify_env(t_env *list, int flag);

char	*get_exec_path(char *env_path, char *cmd);
void	call_execve(char **args, t_env *env);

int		set_cli(t_token *tok, t_cli *cli);
int		executor_constructor(t_token *tok);
void	set_fd(t_token *tok, t_cli *cli);
int		mother_forker(t_cli *commands, pid_t *forked, int amount);
int		get_heredoc(t_token *tok, t_cli *cli);
void	execute_a_command(t_cli *commands);
int		run_commands(void);

int		is_bracket(char c);
int		is_pipe(char c);
int		is_builtin(char *cmd);
int		is_redirector(char *red);
int		is_var(char var);
int		is_quote(char quote);

int		has_var(char *str);
void	free_pbox(char **pbox, int size);
void	null_pbox(char **pbox, int size);
char	*copy_str(char *input, int start, int len);
void	get_quote(char *input, int *i);

int		validate_input(char *input);
void	quick_sort(char **strings, int low, int high);
int		print_error(char *msg, char *refstr, char refchar);

void	link_token(t_token *current, t_token *last);
t_token	*remove_token(t_token *node);
char	*set_pipe_token(char *input, int *i, t_token *token);
char	*set_redirector_token(char *input, int *i, t_token *token);
char	*set_str_token(char *input, int *i);
char	*set_quoted_token(char *input, int *i);
char	*set_expanded_token(char *input, int *i);

#endif