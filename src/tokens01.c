/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens01.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgrabows <fgrabows@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 14:32:49 by fgrabows          #+#    #+#             */
/*   Updated: 2024/09/25 17:03:47 by fgrabows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_append_redir(char *input, int *i, t_token **tokens, char *str);

int ft_is_redir(char *input, int *i, t_token **tokens)
{
	int error;
	char *str;
	
	if (ft_strchr("<>", input[*i]))
	{
		if (input[*i] == input [*i + 1])
		{
			if(ft_append_redir(input, i, tokens, str) == -1)
				return(-1);
			return (0);
		}
		if (input[*i] ==  '<')
			str = ft_strdup("<");
		else
			str = ft_strdup(">");
		if (!str)
			return (ft_perror_message());
		error = create_token(str, T_REDIR, tokens);
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
