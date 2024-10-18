/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_sign.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgrabows <fgrabows@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 12:29:34 by fgrabows          #+#    #+#             */
/*   Updated: 2024/10/17 21:50:15 by fgrabows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//check if the dollar is valid and get its value
//recreate the word with the value and without dollar variable
int ft_dollar(int *i, char **word, t_data *data)
{
	char *var;
	int value;
	
	value = ft_valid_dollar(i, *word, &var);//mem done
	if (value == -1)
	{
		free(word);
		return(-1);
	}
	else if (value >= 1)
			*i = *i + value;
	else 
	{
		value = ft_expand_var(var, data->envp, word, i);//mem done
		if (value == 0)
		{
			value = ft_change_word(var, word, i, data);//mem done
			if (value == -1)
				return(-1);
		}
		else if (value == -1)
			return(-1);
	}
	return(0);
}

//dolar sie nie rozszerza jezeli po sobie ma bialyznak null lub na koncu cudzyslowia

int ft_valid_dollar(int *i, char *word, char **var)
{
	int n;

	n = 1;
	if (word[*i + n] && word[*i + n] == '$')
	{	
		while (word[*i + n] && word[*i + n++] == '$');
		return (n);
	}
	if (word[*i + n] == '?')
		n++;
	else if(!(word[*i + n]) || (!ft_isalpha(word[*i + n]) && word[*i + n] != '_' ))
		return(1);
	else 
	{
		while(word[*i + n] && (ft_isalnum(word[*i + n]) || word[*i + n] == '_'))
			n++;
	}
	*var = malloc(sizeof(char) * n);
	if(!(*var))
		return(ft_perror_message());
	ft_strlcpy(*var, &word[1 + (*i)], n);
	return(0);
}

int ft_expand_var(char *var, t_envp *env, char **word, int *i)
{
	t_envp *env_var;
	char* str;
	int n;
	int value_len;
	
	env_var = fetch_envp_node(env, var);
	if (!env_var)
		return(0);
	n = ft_strlen(var);//5
	free(var);//new
	value_len = ft_strlen(&env_var->value[n + 1]);//1
	str = malloc(sizeof(char) * (ft_strlen(*word) - n + value_len));// + 1 - 1 for $ and null
	if (!str)
	{
		free(word);
		return (ft_perror_message());
	}
	ft_strlcpy(str, *word, *i + 1);
	ft_strlcpy(&str[*i], &env_var->value[n + 1], value_len + 1);
	ft_strlcpy(&str[*i + value_len], &(*word)[*i + n + 1], ft_strlen(*word) - *i - n + 1);	
	free(*word);
	*word = str;
	*i = *i + value_len - 1;
	return(1);
}

int ft_change_word(char* var, char **word, int *i, t_data *data)
{
	char *new_word;
	int word_len;
	int var_len;

	if(var[0] == '?')
	{
		if(ft_exit_extension(var, word, i, data) == -1)
			return (-1);
		return (0);
	}
	var_len = ft_strlen(var);
	word_len = ft_strlen(*word);
	new_word = malloc(sizeof(char) * (word_len - var_len + 1));
	if (!new_word)
	{
		free(var);
		free(*word);
		return (ft_perror_message());	
	}
	ft_strlcpy(new_word, *word, *i);
	ft_strlcpy(&new_word[*i], &(*word)[*i + var_len + 1], word_len - *i - var_len); 
	free(var);
	free(*word);
	*word = new_word;
	return (0);
}

int ft_exit_extension(char *var, char **word, int *i, t_data *data)
{
	char	*new_word;
	char	*exit_code;
	int		exit_len;
	int		word_len;

	
	free(var);
	exit_code = ft_itoa(data->cmd_exit_status);
	if (!exit_code)
		return(ft_perror_free(NULL, *word, NULL));
	exit_len = ft_strlen(exit_code);
	word_len = ft_strlen(*word);
	new_word = malloc(sizeof(char) * (word_len + exit_len - 1));
	if (!new_word)
		return(ft_perror_free(NULL, *word, exit_code));
	ft_strlcpy(new_word, *word, *i);
	ft_strlcpy(&new_word[*i], exit_code, exit_len + 1);
	ft_strlcpy(&new_word[*i + exit_len], &(*word)[*i + 2], word_len - *i - 1);
	free(*word);
	*word = new_word;
}
