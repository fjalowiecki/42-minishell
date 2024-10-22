/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 12:45:31 by fjalowie          #+#    #+#             */
/*   Updated: 2024/10/22 11:17:59 by fjalowie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../libft/libft.h"

typedef struct s_envp
{
	char			*value;
	struct s_envp	*next;
}	t_envp;

typedef struct s_token
{
	int				type;
	char			*text;
	struct s_token	*next;
}	t_token;

typedef struct s_cmd
{
	char			**cmd;
	char			*infile;
	char			*outfile;
	bool			append;
	bool			here_doc;
	bool			redir_error;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_data
{
	t_cmd		*cmd;
	t_envp		*envp;
	char		**envp_arr;
	char		*line;
	int			cmd_exit_status;
}	t_data;

#define BUFFER_SIZE 10

/*tokens*/
#define T_OUT_REDIR	1 
#define T_IN_REDIR		2
#define T_APPEND		3
#define T_HEREDOC		4
#define T_PIPE			5
#define T_WORD			6 
#define T_ARG			7

/*permissions to file*/
#define READ			1
#define WRITE			2
#define EXECUTE		3
// # define T_O_BRACKET		1 // usless
// # define T_C_BRACKET		2 // usless
// # define T_S_QUOTE		5 // useless
// # define T_D_QUOTE		6 // useless
// # define T_FILE			10 // useless

/* errors */
#define MANY_ARGS_ERR "Error: minishell does not accept arguments"
#define NO_ENVP_ERR "Error: no environment found"
#define NO_CMD_ERR "Error: command not found"
#define READLINE_ERR "Error: readline malfunction"
#define MISS_QUOTE_ERR "Syntax error: missing quote"
#define MISS_CMD_ERR "Syntax error: missing command"
#define HEREDOC_ERR "Error: heredoc malfunction"
#define SANITATION_ERR "Error: line sanitation error"
#define REDIR_TO_OPR "Syntax error: redirection followed by unexpected token"
#define NULL_REDIR "Error: ambiguous redirect"

/* main.c */
void	init(t_data *data, int argc, char **envp);
void	free_resources(t_data *data);

/* envp.c */
t_envp	*fetch_envp(char **envp);
void	free_envp(t_envp *head);
int		append_envp_node(t_envp **head, char *str);
void	remove_envp_node(t_envp **head, t_envp *prev_node);
t_envp	*fetch_envp_node(t_envp *head, char *key);
void	increment_shlvl(t_envp *head);
void	free_shlvl_value(t_envp *head);
char	**convert_envp_llist_to_array(t_envp *head);
t_envp	*fetch_node_before(t_envp **head, char *key);

/* syntax_check00.c */
int		check_syntax(char *line);
int		check_line_if_empty(char *line);
int		check_for_unclosed_quotes(char *line);

/* syntax_check01.c */
int		check_for_missing_command(char *line);

/* sanitation00.c */
void	sanitaze_line(t_data *data);

/* sanitation01.c */
char	*process_str(char *str, char *str_final, int str_final_len);

/* utils.c */
void	go_to_next_quote(char *line, int *i, bool go_back);
void	free_ft_split(char **split);
int		ft_check_access(char *file, int type);


/*error.c*/
void	msg_error(char *err);
int		ft_error_message(char *str, int num);
int		ft_perror_message();
void	file_error_msg(char *filename, char *msg);
int ft_perror_free(char *first, char *second, char *third);

/*tokenizer.c*/
t_token *ft_tokenizer(t_data *data);
void	ft_free_tokens(t_token **tokens);
int 	create_token(char *str, int type, t_token **tokens);


/*tokens00/01.c*/
int		ft_is_redir(char *input, int *i, t_token **tokens);
int		ft_is_pipe(char *input, int *i, t_token **tokens);
int		ft_is_word(char *input, int *i, t_token **tokens, t_data *data);
int		ft_create_word_tok(char *str, int *i, t_token **tokens, t_data *data);
int 	ft_extract_word(char *str, int *n, t_token **tokens, t_data *data);
int		ft_cross_word(char **word, t_token **tokens);
int		ft_cut_token(int *i, char **word, t_token **tokens);
int		ft_clear_quote (int *i, char **word, char del);
void	ft_skip_sq(int *n, char *str);
int		ft_check_for_dollar(char **word, t_token **tokens, t_data *data);

/* dollar_sign.c */
int ft_dollar(int *i, char **word, t_data *data);
int ft_valid_dollar(int *i, char *word, char **var);
int ft_expand_var(char *var, t_envp *env, char **word, int *i);
int ft_change_word(char* var, char **word, int *i, t_data *data);
int ft_exit_extension(char *var, char **word, int *i, t_data *data);


/*helpers.c - TO DELETE*/
void ft_print_token_types(t_token *tokens);
void ft_print_split(char **str);
void ft_print_commands(t_cmd *cmds);
void print_string_with_nulls(const char *str);

/* execution00.c */
void	execute_cmds(t_data *data);
void	recursive_pipeline(int input_fd, t_data *data, t_cmd *cmd_node);

/* execution01.c */
char	*find_cmd_path(t_data *data, char *cmd);

/* fd_handlers.c */
void	duplicate_fds(int input_fd, int output_fd);
int		update_input_fd(t_cmd *cmd, int input_fd);
int		get_output_fd(t_cmd *cmd, int *fd_pipe);
int		get_heredoc(t_cmd *cmd);

/** get_next_line.c **/
char	*get_next_line(int fd);

/* cmds.c */
t_cmd *ft_commands(t_token *tokens);
int ft_pipe(t_token **current_tok, t_token *head_tok, t_cmd **current_cmd, t_cmd *head_cmd);
int ft_redir(t_token **current_tok, t_token *head_tok, t_cmd **current_cmd, t_cmd *head_cmd);
int ft_command(t_token **cur_token, t_token *tokens, t_cmd **cur_command, t_cmd *cmds);
int ft_set_command(t_cmd **commands);
void ft_free_commands(t_cmd **commands);

/* cmds_redirs.c */
int	ft_set_redir(t_token **current_tok, t_cmd *current_cmd);

/* builtins.c */
int	check_for_builtin_and_execute(char **cmd, t_data *data);
void env_bltin(t_data *data);
void exit_bltin(t_data *data);

/* blt_echo */
void	echo_bltin(char **cmd, t_data *data);

/* blt_cd */
int	cd_bltin(char **cmd, t_data *data);

/* blt_pwd*/
void	pwd_bltin();

/* blt_export*/
int export_bltin(char **cmd, t_data *data);
int ft_print_env_var(t_data *data);

/* blt_unset */
int unset_bltin(char **cmd, t_data *data);
void ft_remove_head_node(t_envp **head);

/* signals.c */
void handle_signals();
void set_signals_to_default();
void handle_sigint(int sig);

/* token_cleaner.c */
int ft_check_tokens(t_token **tokens);

