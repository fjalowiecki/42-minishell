/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 12:45:31 by fjalowie          #+#    #+#             */
/*   Updated: 2024/09/25 09:58:37 by fjalowie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../libft/libft.h"

typedef struct s_envp
{
	char			*value;
	struct s_envp	*next;
}	t_envp;

typedef struct s_data
{
	t_envp	*envp;
	char	*line;
}	t_data;


# define NO_ENVP_ERR "No environment found"
# define EMPTY_LINE_ERR "Syntax error: empty or whitespace-only line"
# define MISS_QUOTE_ERR "Syntax error: missing quote"
# define MISS_CMD_ERR "Syntax error: missing command"

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

/* syntax_check.c */
int		check_syntax(char *line);
int		check_line_if_empty(char *line);
int		check_for_unclosed_quotes(char *line);
int		check_for_missing_command(char *line);

/* sanitation.c */
char	*sanitaze_line(char *line);

/* utils.c */
void	go_to_next_quote(char *line, int *i, bool go_back);