/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds_redirs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgrabows <fgrabows@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 18:09:57 by fgrabows          #+#    #+#             */
/*   Updated: 2024/10/23 13:03:34 by fgrabows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_here_doc_cmd(t_token *current_tok, t_cmd *current_cmd, char* str);
static void	ft_append_cmd(t_token *current_tok, t_cmd *current_cmd, char* str);
static void	ft_redir_out_cmd(t_token *current_tok, t_cmd *current_cmd, char* str);
static void	ft_redir_in_cmd(t_token *current_tok, t_cmd *current_cmd, char* str);

int	ft_set_redir(t_token **current_tok, t_cmd *current_cmd)
{
	char	*str;

	if (current_cmd->redir_error == true)
		return (0);
	str = ft_strdup((*current_tok)->next->text);
	if (!str)
		return (ft_perror_message());
	if ((*current_tok)->type == T_IN_REDIR)
		ft_redir_in_cmd(*current_tok, current_cmd, str);
	if ((*current_tok)->type == T_OUT_REDIR)
		ft_redir_out_cmd(*current_tok, current_cmd, str);
	if ((*current_tok)->type ==  T_APPEND)
		ft_append_cmd(*current_tok, current_cmd, str);
	if ((*current_tok)->type == T_HEREDOC)
		ft_here_doc_cmd(*current_tok, current_cmd, str);
	return (0);
}

static void	ft_here_doc_cmd(t_token *current_tok, t_cmd *current_cmd, char* str)
{
	if (current_cmd->infile)
		free(current_cmd->infile);
	current_cmd->infile = str;
	current_cmd->here_doc = true;
}

static void	ft_append_cmd(t_token *current_tok, t_cmd *current_cmd, char* str)
{
	int fd;

	fd = open(str, O_WRONLY | O_APPEND | O_CREAT, 0644);
	if (fd < 0)
	{
		ft_perror_message();
		current_cmd->redir_error = true;
		free(str);
		return ;
	}
	close(fd);	
	if (current_cmd->outfile)
		free(current_cmd->outfile);
	current_cmd->outfile = str;
	current_cmd->append = true;
}

static void	ft_redir_in_cmd(t_token *current_tok, t_cmd *current_cmd, char* str)
{
	if (ft_check_access(str, READ) < 0)
	{
		current_cmd->redir_error = true;
		free(str);
		return ;
	}
	if (current_cmd->infile)
		free(current_cmd->infile);
	current_cmd->infile = str;
	current_cmd->here_doc = false;	
}

static void	ft_redir_out_cmd(t_token *current_tok, t_cmd *current_cmd, char* str)
{
	int	fd;
	
	fd = open(str, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (fd < 0)
	{
		ft_perror_message();
		current_cmd->redir_error = true;
		free(str);
		return ;
	}
	close(fd);	
	if (current_cmd->outfile)
		free(current_cmd->outfile);
	current_cmd->outfile = str;
	current_cmd->append = false;
}
