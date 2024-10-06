/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens00.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgrabows <fgrabows@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 11:28:55 by fgrabows          #+#    #+#             */
/*   Updated: 2024/10/04 21:04:05 by fgrabows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

int ft_is_quote(char *input, int *i, t_token **tokens)
{
	int error;
	char *str;
	
	if (ft_strchr("'\"", input[*i]))
	{
		str = ft_quote_dup(&input[*i], i);
		if (!str)
			return (ft_perror_message());
		if (str[0] == '\'')
			error = create_token(str, T_WORD, tokens);
		else
			error = create_token(str, T_WORD, tokens);
		if (error == -1)
		{
			free(str);
			return(-1);
		}
	}
	return (0);
}
int ft_is_pipe(char *input, int *i, t_token **tokens)
{
	int error;
	char *str;
	
	if (input[*i] == '|')
	{
		str = ft_strdup("|");
		if (!str)
			return (ft_perror_message());
		error = create_token(str, T_PIPE, tokens);
		if (error == -1)
		{	
			free(str);
			return(-1);
		}
		(*i)++;
	}
	return (0);
}
int ft_is_word(char *input, int *i, t_token **tokens)
{
	int error;
	char *str;
	
	if(!(ft_strchr("| <>\"\'", input[*i])))
	{
		str = ft_worddup(&(input[*i]), i);
		if (!str)
			return (ft_perror_message());
		error = create_token(str, T_WORD, tokens);
		if (error == -1)
		{	
			free(str);
			return (-1);
		}
		return (0);
	}
	return (0);
}
char* ft_worddup(char *str, int *i)
{
	int n;
	char *new;

	n = -1;
	if (!str)
		return (NULL);
	while (str[++n] && str[n] != ' ');
	if (n == 0)
		return (NULL);
	new = malloc(sizeof(char) * (n + 1));
	if (!new)
	{
		perror(NULL);
		return (NULL);
	}
	ft_strlcpy(new, str, n + 1);
	*i += n;
	return(new);
}

char* ft_quote_dup(char *str, int *i)
{
	int n;
	char *new;
	char delimiter;

	delimiter = *str;
	n = 0;
	if (!str)
		return (NULL);
	while (str[++n] && str[n] != delimiter);
	if (n == 1 && str[n] == '\0') 
		return (NULL);
	new = malloc(sizeof(char) * n);
	if (!new)
	{
		perror(NULL);
		return (NULL);
	}
	ft_strlcpy(new, (str + 1), n);
	*i += (n + 1);
	return(new);
}
