/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 09:54:38 by fjalowie          #+#    #+#             */
/*   Updated: 2024/09/27 11:50:31 by fjalowie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void go_to_next_quote(char *line, int *i, bool go_back)
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
