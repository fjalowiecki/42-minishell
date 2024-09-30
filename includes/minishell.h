/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 12:45:31 by fjalowie          #+#    #+#             */
/*   Updated: 2024/09/30 13:40:34 by fjalowie         ###   ########.fr       */
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
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_redirs
{
	char	*infile;
	char	*outfile;
	bool	append;
}	t_redirs;

typedef struct s_data
{
	t_cmd		*cmd;
	t_redirs	*redirs;
	t_envp		*envp;
	char		**envp_arr;
	char		*line;
	int			cmd_exit_status;
}	t_data;

/*tokens*/
# define T_O_BRACKET	1
# define T_C_BRACKET	2
# define T_REDIR		3
# define T_PIPE			4
# define T_S_QUOTE		5
# define T_D_QUOTE		6
# define T_WORD			7
# define T_APPEND		8

# define NO_ENVP_ERR "Error: no environment found"
# define EMPTY_LINE_ERR "Syntax error: empty or whitespace-only line"
# define MISS_QUOTE_ERR "Syntax error: missing quote"
# define MISS_CMD_ERR "Syntax error: missing command"
# define HEREDOC_ERR "Error: heredoc malfunction"

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

/* execution.c */
void	recursive_pipeline(int input_fd, t_data *data, t_cmd *cmd_node);
void	execute_cmds(t_data *data);

/* fd_handlers.c */
void	duplicate_fds(t_data *data, int input_fd, int output_fd);
int		get_input_fd(t_redirs *redirs);
int		get_output_fd(t_redirs *redirs);
int		get_heredoc(t_data *data);

/** get_next_line.c **/
char	*get_next_line(int fd);