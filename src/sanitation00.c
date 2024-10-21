/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sanitation00.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 09:55:24 by fjalowie          #+#    #+#             */
/*   Updated: 2024/10/21 19:40:59 by fjalowie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*optimize_str_final(char *str_final)
{
	char	*new_str;

	if (!str_final)
		return (NULL);
	new_str = ft_strdup(str_final);
	free(str_final);
	if (!new_str)
		return (NULL);
	return (new_str);
}

char	*sanitaze_line(char *line)
{
	char	*str;
	char	*str_final;
	int		str_final_len;

	str = NULL;
	str = ft_strtrim(line, " \t");
	if (!str)
		return (NULL);
	str_final = malloc(sizeof(char) * (ft_strlen(str) * 2));
	if (!str_final)
	{
		free(str);
		msg_error(SANITATION_ERR);
		return (NULL);
	}
	str_final_len = ft_strlen(str) * 2;
	str_final = process_str(str, str_final, str_final_len);
	str_final = optimize_str_final(str_final);
	if (!str_final)
	{
		free(str);
		msg_error(SANITATION_ERR);
		return (NULL);
	}
	return (str_final);
}
