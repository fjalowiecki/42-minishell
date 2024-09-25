/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 11:33:58 by fjalowie          #+#    #+#             */
/*   Updated: 2024/09/25 09:57:09 by fjalowie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_syntax(char *line)
{
	if (check_line_if_empty(line))
	{
		perror(EMPTY_LINE_ERR);
		return (-1);
	}
	if (check_for_unclosed_quotes(line))
	{
		perror(MISS_QUOTE_ERR);
		return (-1);
	}
	if (check_for_missing_command(line))
	{
		perror(MISS_CMD_ERR);
		return (-1);
	}
	return (0);
}

int check_line_if_empty(char *line)
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
	return (0);
}

int	check_for_unclosed_quotes(char *line)
{
	int		i;
	bool	in_quote;
	char	quote;

	i = 0;
	in_quote = false;
	while (line[i] != '\0')
	{
		if ((line[i] == '\'' || line[i] == '"') && in_quote == false)
		{
			quote = line[i];
			in_quote = true;
		}
		else if (line[i] == quote && in_quote == true)
			in_quote = false;
		i++;
	}
	if (in_quote == true)
		return (-1);
	return (0);
}

int	check_for_following_command(char *line, int i)
{
	i++;
	while (line[i] != '\0')
	{
		if (ft_isspace(line[i]))
			i++;
		else if (ft_isalpha(line[i]))
			return (0);
		else
			break ;
	}
	return (-1);
}

int	check_for_preceding_command(char *line, int i)
{
	char	quote;
	char	init_char;

	init_char = line[i];
	while (i >= 0)
	{
		i--;
		if (i && ((line[i] == '<' && init_char == '<')
				|| (line[i] == '>' && init_char == '>')))
			continue ;
		else if (i && ft_isspace(line[i]))
			continue ;
		else if (i && (line[i] == '"' || line[i] == '\''))
			go_to_next_quote(line, &i, true);
		else if (ft_isalpha(line[i]))
			return (0);
		else
			break ;
	}
	return (-1);
}

int	check_for_missing_command(char *line)
{
	int	i;

	i = 0;
	while (line[i] != '\0')
	{
		if ((line[i] == '"' || line[i] == '\''))
			go_to_next_quote(line, &i, false);
		if (line[i] == '>' && line[i + 1] != '>' || line[i] == '|')
		{
			if (check_for_following_command(line, i))
				return (-1);
			if (check_for_preceding_command(line, i))
				return (-1);
		}
		else if (line[i] == '<' && line[i + 1] != '<')
			if (check_for_following_command(line, i))
				return (-1);
		i++;
	}
	return (0);
}
