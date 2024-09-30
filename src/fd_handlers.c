/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_handlers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 10:56:00 by fjalowie          #+#    #+#             */
/*   Updated: 2024/09/30 13:39:22 by fjalowie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	duplicate_fds(t_data *data, int input_fd, int output_fd)
{
	if (input_fd > 0)
	{
		if (dup2(input_fd, STDIN_FILENO) < 0)
			perror("dup2 failed");
	}
	if (output_fd > 2)
	{
		if (dup2(output_fd, STDOUT_FILENO) < 0)
			perror("dup2 failed");
	}
}

int	get_input_fd(t_redirs *redirs)
{
	int	input_fd;

	if (!redirs->infile)
		return (0);
	input_fd = open(redirs->infile, O_RDONLY);
	if (input_fd < 0)
		perror(redirs->infile);
	return (input_fd);
}

int	get_output_fd(t_redirs *redirs)
{
	int	output_fd;

	if (redirs->append == false)
		output_fd = open(redirs->outfile,
				O_WRONLY | O_CREAT | O_TRUNC, 0664);
	else
		output_fd = open(redirs->outfile,
				O_WRONLY | O_CREAT | O_APPEND, 0664);
	if (output_fd < 0)
		perror(redirs->outfile);
	return (output_fd);
}

static char *get_eof(char *line)
{
	char *eof_start;
	char *eof_end;
	char *eof;
	size_t eof_len;
	
	eof_start = ft_strchr(line, ' ');
	if (!eof_start)
		return (NULL);
	eof_start++;
	eof_end = ft_strchr(eof_start, ' ');
	if (!eof_end)
		return (NULL);
	eof_len = eof_end - eof_start;
	eof = malloc(sizeof(char) * (eof_len + 1));
	if (!eof)
		return (NULL);
	ft_strlcpy(eof, eof_start, eof_len + 1);
	return (eof);
}

int	get_heredoc(t_data *data)
{
	int		fd_pipe[2];
	char	*input;
	char	*eof;

	eof = get_eof(data->line);
	if (!eof)
		return (-1);
	pipe(fd_pipe);
	while (1)
	{
		input = get_next_line(STDIN_FILENO);
		if (ft_strncmp(input, eof, ft_strlen(eof)) == 0)
			break ;
		write(fd_pipe[1], input, ft_strlen(input));
	}
	free(eof);
	close(fd_pipe[1]);
	return (fd_pipe[0]);
}
