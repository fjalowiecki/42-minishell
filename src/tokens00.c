/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens00.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgrabows <fgrabows@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 11:28:55 by fgrabows          #+#    #+#             */
/*   Updated: 2024/10/23 21:09:38 by fgrabows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

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
			return (-1);
		}
		(*i)++;
	}
	return (0);
}

int ft_is_word(char *input, int *i, t_token **tokens, t_data *data)
{	
	if (!(ft_strchr("| <>", input[*i])))
	{
		if (ft_create_word_tok(input, i,tokens, data) == -1)
		{
			free(input);		
			return (-1);
		}
	}
	return (0);
}
int ft_create_word_tok(char *str, int *i, t_token **tokens, t_data *data)
{
	int n;

	n = 0;
	while (str[*i + n] && str[*i + n] != ' ')
	{
		if (str[*i + n] == '\'')
			ft_skip_sq(&n, &str[*i]);
		else if (str[*i + n] == '\"')
		{
			n++;
			while (str[*i + n] && str[*i + n] != '\"')
				n++;
			n++;
		}
		else 
		{
			while(str[*i + n] && !ft_strchr("\"\' ", str[*i + n]))
				n++;
		}
	}
	if (ft_extract_word(&str[*i], &n, tokens, data) == -1)
		return (-1);
	*i = *i + n;
	return (0);
}

//allocate memory for the whole potencial token string 
//and check if there are any dollars to expand 
//if error or end of function free word mem
int ft_extract_word(char *str, int *n, t_token **tokens, t_data *data)
{
	char *word;
	int value;

	word = malloc(sizeof(char) * (*n + 1));
	if (!word)
		return (ft_perror_message());
	ft_strlcpy(word, str, (*n) + 1);
	if(ft_check_for_dollar(&word, data) == -1)
		return (-1);
	value = ft_cross_word(&word, tokens);
	if (value == -1)
	{
		free(word);
		return (-1);
	}
	else if (value == 0)
		return (0);
	if(create_token(word, T_WORD, tokens) == -1)
	{
		free(word);	
		return (-1);
	}
	return (0);
}
//serch for dollars outside of single quotes and extend them to value
//then cross the word to delete the quotes
//create token
// int ft_check_for_dollar(char **word, t_data *data)
// {
// 	int i;
// 	int value;
	
// 	i = 0;
// 	while ((*word)[i])
// 	{
// 		if ((*word)[i] == '\'')
// 			ft_skip_sq(&i, *word);
// 		else if ((*word)[i] == '$')
// 		{
// 			value = ft_dollar(&i, word, data);
// 			if (value == -1)
// 				return (-1);
// 		}
// 		else
// 			i++;
// 	}
// 	return (0);
// }

int ft_check_for_dollar(char **word,  t_data *data)
{
	int i;
	
	i = 0;
	while ((*word)[i])
	{
		if ((*word)[i] == '\'')
			ft_skip_sq(&i, *word);
		else if ((*word)[i] == '$')
		{
			if(ft_dollar(&i, word, data) == -1)
				return (-1);
		}
		else if ((*word)[i] == '\"')
		{
			if (ft_cross_dq (&i, word, data) == -1)
				return (-1);
		}
		else
			i++;
	}
	return (0);
}
