/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgrabows <fgrabows@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 12:45:31 by fjalowie          #+#    #+#             */
/*   Updated: 2024/10/07 09:01:52 by fgrabows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
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

/*tokens*/
# define T_OUT_REDIR	1 
# define T_IN_REDIR		2
# define T_APPEND		3
# define T_HEREDOC		4
# define T_PIPE			5
# define T_WORD			6 
// # define T_O_BRACKET		1 // usless
// # define T_C_BRACKET		2 // usless
// # define T_S_QUOTE		5 // useless
// # define T_D_QUOTE		6 // useless
// # define T_FILE			10 // useless

# define NO_ENVP_ERR "Error: no environment found"
# define EMPTY_LINE_ERR "Syntax error: empty or whitespace-only line"
# define MISS_QUOTE_ERR "Syntax error: missing quote"
# define MISS_CMD_ERR "Syntax error: missing command"
# define HEREDOC_ERR "Error: heredoc malfunction"
# define REDIR_TO_OPR " Syntax error: redirection followed by unexpected token"


# define BUFFER_SIZE 10

/* main.c */
void	init(t_data *data, char **envp);
void	free_resources(t_data *data);

/* envp.c */
t_envp	*fetch_envp(char **envp);
void	free_envp(t_envp *head);
void	append_envp_node(t_envp *head, char *str);
void	remove_envp_node(t_envp *prev_node);
t_envp	*fetch_envp_node(t_envp *head, char *key);
void	increment_shlvl(t_envp *head);
void	free_shlvl_value(t_envp *head);
char	**convert_envp_llist_to_array(t_envp *head);

/* syntax_check.c */
int		check_syntax(char *line);
int		check_line_if_empty(char *line);
int		check_for_unclosed_quotes(char *line);
int		check_for_missing_command(char *line);

/* sanitation.c */
char	*sanitaze_line(char *line);

/* utils.c */
void	go_to_next_quote(char *line, int *i, bool go_back);
void	free_ft_split(char **split);

/*error.c*/
int		ft_error_message(char *str, int num);
int		ft_perror_message();
void	file_error_msg(char *filename, char *msg);

/*tokenizer.c*/
t_token	*ft_tokenizer(char *input);
void	ft_free_tokens(t_token **tokens);
int 	create_token(char *str, int type, t_token **tokens);


/*tokens00/01.c*/
int		ft_is_bracket(char *input, int *i, t_token **tokens);
int		ft_is_redir(char *input, int *i, t_token **tokens);
int		ft_is_quote(char *input, int *i, t_token **tokens);
int		ft_is_pipe(char *input, int *i, t_token **tokens);
int		ft_is_word(char *input, int *i, t_token **tokens);
char*	ft_worddup(char *str, int *i);
char* 	ft_quote_dup(char *str, int *i);


/*helpers.c - TO DELETE*/
void ft_print_token_types(t_token *tokens);
void ft_print_split(char **str);
void ft_print_commands(t_cmd *cmds);


/* execution.c */
void	recursive_pipeline(int input_fd, t_data *data, t_cmd *cmd_node);
void	execute_cmds(t_data *data);

/* fd_handlers.c */
void	duplicate_fds(t_data *data, int input_fd, int output_fd);
//int		get_input_fd(t_redirs *redirs);
//int		get_output_fd(t_redirs *redirs);
int		get_heredoc(t_data *data);

/** get_next_line.c **/
char	*get_next_line(int fd);

/* cmds.c */
t_cmd *ft_commands(t_token *tokens);
int ft_pipe(t_token **current_tok, t_token *head_tok, t_cmd **current_cmd, t_cmd *head_cmd);
int ft_redir(t_token **current_tok, t_token *head_tok, t_cmd **current_cmd, t_cmd *head_cmd);
int	ft_set_redir(t_token **current_tok, t_cmd *current_cmd);
int ft_command(t_token **cur_token, t_token *tokens, t_cmd **cur_command, t_cmd *cmds);
int ft_set_command(t_cmd **commands);
void ft_free_commands(t_cmd **commands);

