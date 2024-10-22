/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 11:06:05 by fgrabows          #+#    #+#             */
/*   Updated: 2024/10/22 09:33:00 by fjalowie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*if error we need to free:
- input
- tokens
- allocated strings in tokens*/


// if error here i am going to return *token with value NULL
// every accessible resource should be freed on error
// at succes returning *tokens and the input gets freed
t_token *ft_tokenizer(t_data *data)
{
	int i;
	t_token *tokens;
	char *input;

	input = data->line;
	tokens = NULL;
	i = 0;
	while(input && input[i])
	{
		if (ft_is_redir(input, &i, &tokens) == -1)
			break;
		if (ft_is_pipe(input, &i, &tokens) == -1)
			break;
		if (ft_is_word(input, &i, &tokens, data) == -1)
			break;
		if (input[i] == ' ')
			i++;
	}
	if (input && input[i])
		ft_free_tokens(&tokens);
	return(tokens);
}
int create_token(char *str, int type, t_token **tokens)
{
	t_token *new_token;
	t_token *current;

	if (!str)
		return(-1);
	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (ft_perror_message());
	new_token->next = NULL;
	new_token->type = type;
	new_token->text = str;
	if(!*tokens)
		*tokens = new_token;
	else 
	{
		current = *tokens;
		while(current->next)
			current = current->next;
		current->next = new_token;
	}
	return (0);
}

void  ft_free_tokens(t_token **tokens)
{
	t_token *temp;
	
	if (!tokens || !*tokens)
		return ;
	while(*tokens)
	{
		temp = *tokens;
		*tokens = (*tokens)->next;
		if (temp->text)
			free(temp->text);
		free(temp); 
	}
	*tokens = NULL;
}
