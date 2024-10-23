/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 19:25:31 by fjalowie          #+#    #+#             */
/*   Updated: 2024/10/23 14:52:41 by fjalowie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*process_buffer_with_eol(char *buffer, char *line)
{
	int		eol_length;
	char	*eol;
	char	*tmp_line;

	eol = ft_strchr(buffer, '\n');
	eol_length = ft_strlen(buffer) - ft_strlen(eol) + 1;
	tmp_line = malloc(sizeof(char) * (ft_strlen(line) + 1 + eol_length));
	if (!tmp_line)
		return (NULL);
	ft_strcpy(tmp_line, line);
	free(line);
	line = tmp_line;
	ft_strncat(line, buffer, eol_length);
	eol++;
	while (*eol != '\0')
		*buffer++ = *eol++;
	buffer[ft_strlen(eol)] = '\0';
	return (line);
}

static char	*process_buffer_without_eol(char *buffer, char *line)
{
	char	*tmp_line;
	int		j;

	tmp_line = malloc(sizeof(char) * (ft_strlen(line) + BUFFER_SIZE + 1));
	if (!tmp_line)
		return (NULL);
	ft_strcpy(tmp_line, line);
	free(line);
	line = tmp_line;
	ft_strncat(line, buffer, ft_strlen(buffer));
	j = 0;
	while (j < BUFFER_SIZE + 1)
	{
		buffer[j] = '\0';
		j++;
	}
	return (line);
}

static int	process_nb_read(int fd, char *buffer, char *line)
{
	int	nb_read;

	nb_read = read(fd, buffer, BUFFER_SIZE);
	if (nb_read <= 0 && *line == 0)
	{
		free(line);
		return (-1);
	}
	buffer[nb_read] = '\0';
	return (nb_read);
}

char	*get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE + 1];
	char		*line;
	int			nb_read;

	line = (char *)ft_calloc((BUFFER_SIZE + 1), sizeof(char));
	if (!line)
		return (NULL);
	nb_read = 1;
	while (nb_read != 0)
	{
		if (*buffer == '\0')
		{
			nb_read = process_nb_read(fd, buffer, line);
			if (nb_read == -1)
				return (NULL);
		}
		if (ft_strchr(buffer, '\n'))
		{
			line = process_buffer_with_eol(buffer, line);
			break ;
		}
		else
			line = process_buffer_without_eol(buffer, line);
	}
	return (line);
}
