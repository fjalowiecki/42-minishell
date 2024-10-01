/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgrabows <fgrabows@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 11:13:47 by fgrabows          #+#    #+#             */
/*   Updated: 2024/10/01 20:12:46 by fgrabows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//if error ocures everything is freed
//if calling some additional function end with error all memory is going to be freed
//if all called functions ends with succes all memory but *commands is going to be freed
t_cmd *ft_commands(t_token *tokens)
{
	t_cmd *commands;
	t_cmd *current_cmd;
	t_token *current_tok;
	
	commands = NULL;
	current_tok = tokens;
	if(ft_set_command(&commands) == -1)
	{
		ft_free_tokens(&tokens);
		return(NULL);
	}
	current_cmd = commands;
	while (current_tok)
	{
		if(ft_redir(&current_tok, tokens, current_cmd, commands) == -1)
			return NULL;
		if(ft_command(&current_tok, tokens, current_cmd, commands) == -1)
			return NULL;
		if(ft_pipe(&current_tok, tokens, current_cmd, commands) == -1)
			return NULL;
	}
	if(!tokens)
		return(NULL);
	ft_free_tokens(&tokens);
	return (commands);
}
int ft_pipe(t_token **current_tok, t_token *head_tok, t_cmd **current_cmd, t_cmd *head_cmd)
{
	if (!*current_tok)
		return (0);
	if((*current_tok)->type == T_PIPE)
	{
		if(ft_set_command(current_cmd) == -1)
		{
			ft_free_tokens(&head_tok);
			ft_free_commands(&head_cmd);
			return(-1);
		}
	}
	return (0);
}
int ft_redir(t_token **current_tok, t_token *head_tok, t_cmd **current_cmd, t_cmd *head_cmd)
{
	if (!*current_tok)
		return (0);
	if((*current_tok)->type == T_IN_REDIR || (*current_tok)->type == T_APPEND || (*current_tok)->type == T_OUT_REDIR)
	{
		if((*current_tok)->next && ((*current_tok)->next->type == T_D_QUOTE  
			|| (*current_tok)->next->type == T_WORD || (*current_tok)->next->type == T_S_QUOTE))
		{
			if (ft_set_redir(current_tok, *current_cmd) == -1)
			{
				ft_free_tokens(&head_tok);
				ft_free_commands(&head_cmd);
				return(-1);
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
	return(0);
}

int	ft_set_redir(t_token **current_tok, t_cmd *current_cmd)
{
	char *str;

	str = ft_strdup((*current_tok)->next->text);
	if (!str)
		return (ft_perror_message());
	if ((*current_tok)->type == T_IN_REDIR)
		current_cmd->infile = str;
	if ((*current_tok)->type == T_OUT_REDIR)
	{
		current_cmd->infile = str;
		current_cmd->append = false;
	}
	if ((*current_tok)->type ==  T_APPEND)
	{
		current_cmd->infile = str;
		current_cmd->append = true;
	}
	(*current_tok) = (*current_tok)->next->next;
	return(0);
}

int ft_command(t_token **tokens, t_token *head_tok, t_cmd *cmd, t_cmd *head_cmd)
{
	int i;
	
	if (!(*tokens))
		return (0);
	if ((*tokens)->type == T_D_QUOTE || (*tokens)->type == T_S_QUOTE || (*tokens)->type == T_WORD)
	{
		i = ft_count_tok(*tokens);
		if (ft_create_cmds(tokens, cmd, i) == -1)
		{
			ft_free_tokens(&head_tok);
			ft_free_commands(&head_cmd);
			return (-1);
		}
	}
	return (0);
}
static int ft_create_cmds(t_token **tokens, t_cmd *commands, int i)
{
	char **cmds;
	char *arg;
	int n;
	
	n = 0;
	cmds = malloc(sizeof(char *) * (i + 1));
	if (!cmds)
		return(ft_perror_message());
	while(*tokens && ((*tokens)->type == T_D_QUOTE || (*tokens)->type == T_S_QUOTE || (*tokens)-> type == T_WORD))
	{
		arg = ft_strdup((*tokens)->text);
		if(!arg)
		{
			ft_free_args(n, cmds);
			return (ft_perror_message());
		}
		cmds[n++] = arg;
		*tokens = (*tokens)->next;
	}
	cmds[n] = NULL;
	return(0);
}
static void ft_free_args(int i, char **cmds)
{
	int x;
	
	x = 0;
	while(x < i)
		free(cmds[x++]);
	free(cmds);
}

static int ft_count_tok(t_token *tokens)
{
	int i;
	
	i = 0;
	while(tokens && (tokens->type == T_D_QUOTE || tokens->type == T_S_QUOTE || tokens-> type == T_WORD))
	{
		i++;
		tokens = tokens->next;
	}
	return(i);
}
// dodanie srodowiska do cmd - do dodania
int ft_set_command(t_cmd **commands)
{
	t_cmd *new;
	t_cmd *tmp;
	
	new = malloc(sizeof(t_cmd));
	if (!new)
		return(ft_perror_message());
	new->append = false;
	new->infile = NULL;
	new->outfile = NULL;
	new->next = NULL;
	new->envp = NULL;
	new->cmd = NULL;
	if(!(*commands))
		*commands = new;
	else
	{
		tmp = *commands;
		while(tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	return(0);
}

void ft_free_commands(t_cmd **commands)
{
	t_cmd *temp;
	
	if(!commands)
		return ;
	while (*commands)
	{
		temp = *commands; 
		if(temp->infile)
			free(temp->infile);
		if(temp->outfile)
			free(temp->outfile);
		if(temp->cmd)
			free_ft_split(temp->cmd);
		*commands = (*commands)->next;
		free(temp);
	}
}


