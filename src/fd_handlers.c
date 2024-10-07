/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_handlers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 10:56:00 by fjalowie          #+#    #+#             */
/*   Updated: 2024/10/07 12:15:02 by fjalowie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	duplicate_fds(int input_fd, int output_fd)
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

int	update_input_fd(t_cmd *cmd, int input_fd)
{
	if (cmd->here_doc)
	{
		input_fd = get_heredoc(cmd);
		if (input_fd < 0)
			ft_error_message(HEREDOC_ERR, -1);
		return (input_fd);
	}
	else if (cmd->infile)
	{
		input_fd = open(cmd->infile, O_RDONLY);
		if (input_fd < 0)
			perror(cmd->infile);
		return (input_fd);
	}
	return (input_fd);
}

int	get_output_fd(t_cmd *cmd, int *fd_pipe)
{
	int	output_fd;

	if (cmd->outfile && cmd->append == false)
		output_fd = open(cmd->outfile,
				O_WRONLY | O_CREAT | O_TRUNC, 0664);
	else if (cmd->outfile && cmd->append == true)
		output_fd = open(cmd->outfile,
				O_WRONLY | O_CREAT | O_APPEND, 0664);
	else if (cmd->next == NULL)
		output_fd = STDOUT_FILENO;
	else 
		output_fd = fd_pipe[1];
	if (output_fd < 0 && cmd->outfile)
		perror(cmd->outfile);
	else if (output_fd < 0 && !cmd->outfile)
		perror("output_fd");
	return (output_fd);
}

/* static char *get_eof(char *line)
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
} */

int	get_heredoc(t_cmd *cmd)
{
	int		fd_pipe[2];
	char	*input;
	char	*eof;

	eof = cmd->infile;
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
	close(fd_pipe[1]);
	return (fd_pipe[0]);
}
