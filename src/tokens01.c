/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens01.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgrabows <fgrabows@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 14:32:49 by fgrabows          #+#    #+#             */
/*   Updated: 2024/10/01 17:54:16 by fgrabows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_append_redir(char *input, int *i, t_token **tokens, char *str);
static int ft_single_redirection(char x, t_token **tokens, char *str);

int ft_is_redir(char *input, int *i, t_token **tokens)
{
	int error;
	char *str;
	
	str = NULL;
	if (ft_strchr("<>", input[*i]))
	{
		if (input[*i] == input [*i + 1])
		{
			if(ft_append_redir(input, i, tokens, str) == -1)
				return(-1);
			return (0);
		}
		error = ft_single_redirection(input [*i], tokens, str);
		(*i)++;
		if (error == 0)	
			return (0);	
		free(str);
		return(-1);
	}
	return (0);
}

static int	ft_append_redir(char *input, int *i, t_token **tokens, char *str)
{
	if (input [*i] == '<')
		str = ft_strdup("<<");
	else 
		str = ft_strdup(">>");
	if (!str)
		return (ft_perror_message());
	if (create_token(str, T_APPEND, tokens) == -1)
	{
		free(str);
		return (-1);
	}
	*i =  *i + 2;
	return (0);
}

static int ft_single_redirection(char x, t_token **tokens, char *str)
{
	int type;
	int error;
	
	if (x == '<')
	{
		type = T_IN_REDIR;
		str = ft_strdup("<");
	}
	else
	{
		type = T_OUT_REDIR;
		str = ft_strdup(">");
	}
	if (!str)
		return (ft_perror_message());
	error = create_token(str, type, tokens);
	free(str);
	if (error == -1);
		return(-1);
	return (0);
}
