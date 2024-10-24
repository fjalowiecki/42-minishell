/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds_00.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgrabows <fgrabows@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 11:13:47 by fgrabows          #+#    #+#             */
/*   Updated: 2024/10/24 09:40:34 by fgrabows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
if error ocures everything is freed
if the calling of additional function ends 
	with error all memory is going to be freed
if all called functions ends with succes all
	 memory but *commands is going to be freed
*/
int	ft_cmds_creation(t_data *data)
{
	t_token	*tokens;

	tokens = ft_tokenizer(data, data->line);
	if (tokens == NULL)
	{
		data->cmd_exit_status = 1;
		return (-1);
	}
	if (ft_check_tokens(&tokens) == -1)
	{
		data->cmd_exit_status = 1;
		return (-1);
	}
	if (tokens == NULL)
		return (-1);
	data->cmd = ft_commands(tokens);
	if (data->cmd == NULL)
	{
		data->cmd_exit_status = 1;
		return (-1);
	}
	return (0);
}

t_cmd	*ft_commands(t_token *tokens)
{
	t_cmd	*commands;
	t_cmd	*current_cmd;
	t_token	*current_tok;

	commands = NULL;
	current_tok = tokens;
	if (ft_set_command(&commands) == -1)
	{
		ft_free_tokens(&tokens);
		return (NULL);
	}
	current_cmd = commands;
	while (current_tok)
	{
		if (ft_redir(&current_tok, tokens, &current_cmd, commands) == -1)
			return (NULL);
		if (ft_command(&current_tok, tokens, &current_cmd, commands) == -1)
			return (NULL);
		if (ft_pipe(&current_tok, tokens, &current_cmd, commands) == -1)
			return (NULL);
	}
	if (!tokens)
		return (NULL);
	ft_free_tokens(&tokens);
	return (commands);
}

int	ft_pipe(t_token **current_tok, t_token *head_tok,
		t_cmd **current_cmd, t_cmd *head_cmd)
{
	if (!*current_tok)
		return (0);
	if ((*current_tok)->type == T_PIPE)
	{
		if (ft_set_command(current_cmd) == -1)
		{
			ft_free_tokens(&head_tok);
			ft_free_commands(&head_cmd);
			return (-1);
		}
		(*current_tok) = (*current_tok)->next;
		(*current_cmd) = (*current_cmd)->next;
	}
	return (0);
}

int	ft_redir(t_token **current_tok, t_token *head_tok,
		t_cmd **current_cmd, t_cmd *head_cmd)
{
	if (!*current_tok)
		return (0);
	if ((*current_tok)->type != T_WORD && (*current_tok)->type != T_PIPE)
	{
		if ((*current_tok)->next && (*current_tok)->next->type == T_WORD)
		{
			if (ft_set_redir(current_tok, *current_cmd) == -1)
			{
				ft_free_tokens(&head_tok);
				ft_free_commands(&head_cmd);
				return (-1);
			}
			(*current_tok) = (*current_tok)->next->next;
		}
		else
		{
			ft_free_tokens(&head_tok);
			ft_free_commands(&head_cmd);
			return (ft_error_message(REDIR_TO_OPR, -1));
		}
	}
	return (0);
}
