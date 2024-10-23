/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check00.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 11:33:58 by fjalowie          #+#    #+#             */
/*   Updated: 2024/10/23 19:07:34 by fjalowie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_syntax(char *line)
{
	if (check_line_if_empty(line))
		return (-1);
	if (check_for_unclosed_quotes(line))
		return (ft_error_message(MISS_QUOTE_ERR, -1));
	if (check_for_missing_command(line))
		return (ft_error_message(MISS_CMD_ERR, -1));
	return (0);
}

int	check_line_if_empty(char *line)
{
	char	*trimmed_line;

	trimmed_line = ft_strtrim(line, " \t");
	if (!trimmed_line)
	{
		perror("check_line_if_empty");
		return (1);
	}
	if (ft_strlen(trimmed_line) == 0)
	{
		free(trimmed_line);
		return (-1);
	}
	free(trimmed_line);
	return (0);
}

int	check_for_unclosed_quotes(char *line)
{
	int		i;
	bool	in_quote;
	char	quote;

	i = 0;
	in_quote = false;
	quote = '\0';
	while (line[i] != '\0')
	{
		if ((line[i] == '\'' || line[i] == '"'))
		{
			if (!in_quote)
			{
				quote = line[i];
				in_quote = true;
			}
			else if (line[i] == quote)
				in_quote = false;
		}
		i++;
	}
	if (in_quote == true)
		return (-1);
	return (0);
}
