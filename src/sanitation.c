/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sanitation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 09:55:24 by fjalowie          #+#    #+#             */
/*   Updated: 2024/10/10 09:02:42 by fjalowie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_quotes(char *str, char *str_final, int *i, int *j)
{
	int	i_cp;

	i_cp = *i;
	go_to_next_quote(str, i, false);
	(*i)++;
	ft_strlcpy(&str_final[*j], &str[i_cp], *i - i_cp + 1);
	*j += *i - i_cp;
}

static void	handle_operators(const char *str, char *str_final, int *i, int *j)
{
	str_final[(*j)++] = str[(*i)++];
	str_final[(*j)++] = ' ';
}

static int	realloc_str_final(char **str_final, int j, int str_final_len)
{
	char	*new_str;

	new_str = malloc(sizeof(char) * (str_final_len * 2));
	if (!new_str)
		return (-1);
	ft_strlcpy(new_str, *str_final, j);
	free(*str_final);
	*str_final = new_str;
	return (str_final_len * 2);
}

static char	*optimize_str_final(char *str_final)
{
	char	*new_str;

	new_str = ft_strdup(str_final);
	if (!new_str)
		return (NULL);
	free(str_final);
	return (new_str);
}

static char	*process_str(char *str, char *str_final, int str_final_len)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i] != '\0')
	{
		if (j + 1 >= str_final_len)
		{
			str_final_len = realloc_str_final(&str_final, j, str_final_len);
			if (str_final_len == -1)
				return (NULL);
		}
		if (str[i] == '"' || str[i] == '\'')
			handle_quotes(str, str_final, &i, &j);
		else if (ft_isspace(str[i]) && (i == 0 || ft_isspace(str[i - 1])))
			i++;
		else if ((ft_isalnum(str[i]) && (str[i + 1] == '>' || str[i + 1] == '<' || str[i + 1] == '|'))
			|| (str[i] == '>' || str[i] == '<' || str[i] == '|') && ft_isalpha(str[i + 1]))
			handle_operators(str, str_final, &i, &j);
		else
			str_final[j++] = str[i++];
	}
	str_final[j] = '\0';
	return (str_final);
}

char	*sanitaze_line(char *line)
{
	char	*str;
	char	*str_final;
	int		str_final_len;

	str = ft_strtrim(line, " \t");
	if (!str)
		return (NULL);
	str_final = malloc(sizeof(char) * (ft_strlen(str) * 2));
	if (!str_final)
	{
		free(str);
		return (NULL);
	}
	str_final_len = ft_strlen(str) * 2;
	str_final = process_str(str, str_final, str_final_len);
	if (!str_final)
		return (NULL);
	free(str);
	str_final = optimize_str_final(str_final);
	if (!str_final)
		return (NULL);
	return (str_final);
}
