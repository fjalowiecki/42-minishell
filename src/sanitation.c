/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sanitation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 09:55:24 by fjalowie          #+#    #+#             */
/*   Updated: 2024/10/09 13:02:38 by fjalowie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void handle_quotes(char *trim, char *final, int *i, int *j)
{
    int i_cp;
	
	i_cp = *i;
    go_to_next_quote(trim, i, false);
    (*i)++;
    ft_strlcpy(&final[*j], &trim[i_cp], *i - i_cp + 1);
    *j += *i - i_cp;
}

static void handle_operators(const char *trim, char *final, int *i, int *j)
{
    final[(*j)++] = trim[(*i)++];
    final[(*j)++] = ' ';
}

char *sanitaze_line(char *line)
{
	char *trim;
	char *final;
	int i;
	int j;

	trim = ft_strtrim(line, " \t");
	if (!trim)
		return (NULL);
	final = malloc(sizeof(char) * (ft_strlen(trim) + 1));
	if (!final)
	{
		free(trim);
		return (NULL);
	}
	i = 0;
	j = 0;
	while(trim[i] != '\0')
	{
		if (trim[i] == '"' || trim[i] == '\'')
			handle_quotes(trim, final, &i, &j);
		else if (ft_isspace(trim[i]) && (i == 0 || ft_isspace(trim[i - 1])))
			i++;
		else if ((ft_isalnum(trim[i]) && (trim[i + 1] == '>' || trim[i + 1] == '<' || trim[i + 1] == '|'))
				|| (trim[i] == '>' || trim[i] == '<' || trim[i] == '|') && ft_isalpha(trim[i + 1]))
			handle_operators(trim, final, &i, &j);
		else
			final[j++] = trim[i++];
	}
	final[j] = '\0';
	free(trim);
	// printf("final: %s\n", final);
	return (final);
}
