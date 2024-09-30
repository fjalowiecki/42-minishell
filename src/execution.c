/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 11:53:52 by fjalowie          #+#    #+#             */
/*   Updated: 2024/09/30 14:33:34 by fjalowie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*find_cmd_path(t_envp *envp, char *cmd)
{
	char	**envp_paths;
	char	*final_envp_path;
	char	*envp_path_part;
	int		i;

	while (envp && ft_strncmp(envp->value, "PATH", 4) != 0)
		envp = envp->next;
	envp_path_part = ft_strjoin("/", cmd);
	envp_paths = ft_split(envp->value + 5, ':');
	i = 0;
	while (envp_paths[i] != NULL)
	{
		final_envp_path = ft_strjoin(envp_paths[i], envp_path_part);
		if (access(final_envp_path, X_OK) == 0)
			break ;
		free(final_envp_path);
		final_envp_path = NULL;
		i++;
	}
	free_ft_split(envp_paths);
	free(envp_path_part);
	return (final_envp_path);
}

static void	process_last_cmd(t_data *data, char **cmd, int input_fd)
{
	int output_fd;
	
	output_fd = 1;
	if (!data->redirs->outfile)
		dup2(input_fd, STDIN_FILENO);
	else
	{
		output_fd = get_output_fd(data->redirs);
		duplicate_fds(data, input_fd, output_fd);
	}
	if (input_fd > 0)
		close(input_fd);
	if (output_fd > 2)
		close(output_fd);
	cmd[0] = find_cmd_path(data->envp, cmd[0]);
	if (cmd[0] != NULL && output_fd > 0)
	{
		if (execve(cmd[0], cmd, data->envp_arr) < 0)
			perror("execve failed");
	}
}

static void	process_cmd(t_data *data, char **cmd, int input_fd, int *fd_pipe)
{
	duplicate_fds(data, input_fd, fd_pipe[1]);
	if (input_fd > 0)
		close(input_fd);
	close(fd_pipe[0]);
	close(fd_pipe[1]);
	cmd[0] = find_cmd_path(data->envp, cmd[0]);
	if (cmd[0] != NULL && input_fd >= 0)
	{
		if (execve(cmd[0], cmd, data->envp_arr) < 0)
			perror("execve failed");
	}
}

void	recursive_pipeline(int input_fd, t_data *data, t_cmd *cmd_node)
{
	int		fd_pipe[2];
	int		status;
	pid_t	pid;

	if (cmd_node->next == NULL)
	{
		pid = fork();
		if (pid < 0)
			perror("fork failed");
		else if (pid == 0)
			process_last_cmd(data, cmd_node->cmd, input_fd);
		else
		{
			waitpid(pid, &status, 0);
			data->cmd_exit_status = WTERMSIG(status);
		}
	}
	else
	{
		pipe(fd_pipe);
		pid = fork();
		if (pid < 0)
			perror("fork failed");
		else if (pid == 0)
			process_cmd(data, cmd_node->cmd, input_fd, fd_pipe);
		else
		{
			close(fd_pipe[1]);
			waitpid(pid, NULL, 0);
			if (input_fd > 0)
				close(input_fd);
			recursive_pipeline(fd_pipe[0], data, cmd_node->next);
		}
	}
}

void	execute_cmds(t_data *data)
{
	int input_fd;

	if (data->envp_arr)
		free_ft_split(data->envp_arr);
	data->envp_arr = convert_envp_llist_to_array(data->envp);
	if (ft_strncmp(data->line, "<<", 2) == 0)
	{
		input_fd = get_heredoc(data);
		if (input_fd < 0)
			ft_error_message(HEREDOC_ERR, -1);
	}
	else
		input_fd = get_input_fd(data->redirs);
	recursive_pipeline(input_fd, data, data->cmd);
}
