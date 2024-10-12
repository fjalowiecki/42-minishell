/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens00.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgrabows <fgrabows@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 11:28:55 by fgrabows          #+#    #+#             */
/*   Updated: 2024/10/12 20:21:20 by fgrabows         ###   ########.fr       */
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
			return(-1);
		}
		(*i)++;
	}
	return (0);
}

int ft_is_word(char *input, int *i, t_token **tokens, t_envp *env)
{	
	if(!(ft_strchr("| <>", input[*i])))
	{
		if (ft_create_word_tok(input, i,tokens, env) == -1)
		{
			free(input);		
			return (-1);
		}
	}
	return (0);
}
int ft_create_word_tok(char *str, int *i, t_token **tokens, t_envp *env)
{
	int n;
	char *dol;

	n = 0;
	while(str[*i + n] && str[*i + n] != ' ')
	{
		if (str[*i + n] == '\'')
			ft_skip_sq(&n, &str[*i + n]);
		if (str[*i + n] == '\"')
		{
			n++;
			while(str[*i + n] && str[*i + n] != '\"')
				n++;
			n++;
		}
		while(str[*i + n] && !ft_strchr("\"\' ", str[*i + n]))
			n++;
	}
	if (ft_extract_word(&str[*i], &n, tokens, env) == -1)
		return (-1);
	*i = *i + n;
	return (0);
}

//allocate memory for the whole potencial token string 
//and check if there are any dollars to expand 
//if error or end of function free word mem
int ft_extract_word(char *str, int *n, t_token **tokens, t_envp *env)
{
	char *word;
	int value;

	word = malloc(sizeof(char) * (*n + 1));
	if (!word)
		return(ft_perror_message());
	ft_strlcpy(word, str, (*n) + 1);
	if(ft_check_for_dollar(&word, tokens, env) == -1)
		return(-1);
	value = ft_cross_word(&word, tokens);
	if (value == -1)
	{
		free(word);
		return (-1);
	}
	else if (value == 0)
		return(0);
	if(create_token(word, T_WORD, tokens) == -1)
	{
		free(word);	
		return(-1);
	}
	return (0);
}
//serch for dollars outside of single quotes and extend them to value
//then cross the word to delete the quotes
//create token
int ft_check_for_dollar(char **word, t_token **tokens, t_envp *env)
{
	int i;
	int value;
	
	i = 0;
	while ((*word)[i])
	{
		if ((*word)[i] == '\'')
			ft_skip_sq(&i, *word);
		else if ((*word)[i] == '$')
		{
			value = ft_dollar(&i, word, env);//extending variables

			if (value == -1)
				return(-1);
		}
		else
			i++;
	}
	return(0);
}
/*cross whole word looking for the quotes and double quotes and delete them
//if white char occures outside of quotes it means that we have to cut the word
//cut the word, create token of the first half and check the second half
//return zero if there is no char after space which occured out of quotes	
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
			if(ft_clear_quote(&i, word, (*word)[i]) == -1)//after ' sign//mem git
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
int ft_cut_token(int *i, char **word, t_token **tokens)
{
	char *tok_str;
	char *new_word;

	tok_str = malloc(sizeof(char) * (*i + 1));
	if (!tok_str)
		return(ft_perror_message());
	ft_strlcpy(tok_str, *word, *i + 1);
	if (create_token(tok_str, T_WORD, tokens) == -1)
	{
		free(tok_str);
		return (-1);//take care
	}
	while ((*word)[*i] && ft_strchr(" \t\r\n\v\f", (*word)[*i]))
		(*i)++;
	if (!(*word)[*i])
	{
		free(*word);
		return (0);
	}
	new_word = malloc(sizeof(char) * (ft_strlen(&(*word)[*i]) + 1));
	if (!new_word)
		return(ft_perror_message());
	ft_strlcpy(new_word, &(*word)[*i], (ft_strlen(&(*word)[*i]) + 1));
	free(*word);
	*word = new_word;
	*i = 0;
	return(1);
}

//clear the word of unnecessary quotes 
int ft_clear_quote (int *i, char **word, char del)
{
	int n;
	char *new_word;
	int len;
	
	n = 1;
	len = ft_strlen(*word);
	while((*word)[*i + n] && (*word)[*i + n] != del)
		n++;
	new_word = malloc(sizeof(char) * (len - 1));
	if (!new_word)
		return(ft_perror_message());
	ft_strlcpy(new_word, *word, *i + 1);
	ft_strlcpy(&new_word[*i], &(*word)[*i + 1], n);
	ft_strlcpy(&new_word[*i + n - 1], &(*word)[*i + n + 1], (len - *i - n));
	free(*word);
	*word = new_word;
	*i = *i + n - 1;
	return(0);
}

void ft_skip_sq(int *n, char *str)
{
	(*n)++;
	while(str[*n] && str[*n] != '\'')
		(*n)++;
	(*n)++;
}
