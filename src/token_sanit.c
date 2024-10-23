/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_sanit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgrabows <fgrabows@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 10:44:21 by fgrabows          #+#    #+#             */
/*   Updated: 2024/10/23 21:19:22 by fgrabows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
cross whole word looking for the quotes and double quotes and delete them
if white char occures outside of quotes it means that we have to cut the word
cut the word, create token of the first half and check the second half
return zero if there is no char after space which occured out of quotes	
*/
int ft_cross_word(char **word, t_token **tokens)
{
	int i;
	int value;

	i = 0;
	while((*word)[i])
	{
		if ((*word)[i] == '\'' || (*word)[i] == '\"')
		{
			if(ft_clear_quote(&i, word, (*word)[i]) == -1)
				return (-1);
		}
		else if (ft_strchr(" \t\r\n\v\f", (*word)[i]))
		{
			value = ft_cut_token(&i, word, tokens);
			if (value == -1)
				return(-1);
			if (value == 0)
				return(0);
		}
		else 
			i++;
	}
	return (1);
}

//create the token of the first half and create new word to check it to the end
int	ft_cut_token(int *i, char **word, t_token **tokens)
{
	char	*tok_str;
	char	*new_word;

	tok_str = malloc(sizeof(char) * (*i + 1));
	if (!tok_str)
		return (ft_perror_message());
	ft_strlcpy(tok_str, *word, *i + 1);
	if (create_token(tok_str, T_WORD, tokens) == -1)
		return (ft_perror_free(tok_str,NULL, NULL));
	while ((*word)[*i] && ft_strchr(" \t\r\n\v\f", (*word)[*i]))
		(*i)++;
	if (!(*word)[*i])
	{
		free(*word);
		return (0);
	}
	new_word = malloc(sizeof(char) * (ft_strlen(&(*word)[*i]) + 1));
	if (!new_word)
		return (ft_perror_message());
	ft_strlcpy(new_word, &(*word)[*i], (ft_strlen(&(*word)[*i]) + 1));
	free(*word);
	*word = new_word;
	*i = 0;
	return (1);
}

//clear the word of unnecessary quotes 
int	ft_clear_quote (int *i, char **word, char del)
{
	int n;
	char *new_word;
	int len;
	
	n = 1;
	len = ft_strlen(*word);
	while ((*word)[*i + n] && (*word)[*i + n] != del)
		n++;
	new_word = malloc(sizeof(char) * (len - 1));
	if (!new_word)
		return (ft_perror_message());
	ft_strlcpy(new_word, *word, *i + 1);
	ft_strlcpy(&new_word[*i], &(*word)[*i + 1], n);
	ft_strlcpy(&new_word[*i + n - 1], &(*word)[*i + n + 1], (len - *i - n));
	free(*word);
	*word = new_word;
	*i = *i + n - 1;
	return (0);
}

void ft_skip_sq(int *n, char *str)
{
	(*n)++;
	while (str[*n] && str[*n] != '\'')
		(*n)++;
	(*n)++;
}

int ft_cross_dq(int *i, char **word, t_data *data)
{
	(*i)++;
	while ((*word)[*i] != '\"')
	{
		if ((*word)[*i] == '$')
		{
			if (ft_dollar(i, word, data) == -1)
				return (-1);
			continue;
		}
		(*i)++;
	}
	(*i)++;
	return (0);
}
