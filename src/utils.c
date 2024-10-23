/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgrabows <fgrabows@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 09:54:38 by fjalowie          #+#    #+#             */
/*   Updated: 2024/10/23 13:08:46 by fgrabows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	go_to_next_quote(char *line, int *i, bool go_back)
{
	char	quote;
	int		j;

	if (!line || !i)
		return;

	quote = line[*i];
	j = *i;
	if (go_back == false)
	{
		j++;
		while (line[j] != '\0' && line[j] != quote)
			j++;
	}
	else
	{
		j--;
		while (j >= 0 && line[j] != quote)
			j--;
	}
	*i = j; 
}

void	free_ft_split(char **split)
{
	char	**orig_split;

	orig_split = split;
	while (*split != NULL)
	{
		free(*split);
		split++;
	}
	free(orig_split);
}

int ft_check_access(char *file, int type)
{
	int value;
	
	if (type == READ)
	{
		value = access(file, F_OK | R_OK);
		if (value < 0)
			return (ft_perror_message());
	}
	else if (type == WRITE)	
	{
		value = access(file, F_OK | W_OK);
		if (value < 0)
			return (ft_perror_message());
	}
	return (0);
}

void	ft_free_commands(t_cmd **commands)
{
	t_cmd	*temp;

	int i = 0;
	if (!commands)
		return ;
	while (*commands)
	{
		temp = *commands; 
		if (temp->infile)
			free(temp->infile);
		if (temp->outfile)
			free(temp->outfile);
		if (temp->cmd)
			free_ft_split(temp->cmd);
		*commands = (*commands)->next;
		free(temp);
	}
}
