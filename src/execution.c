/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 11:53:52 by fjalowie          #+#    #+#             */
/*   Updated: 2024/10/09 11:16:03 by fjalowie         ###   ########.fr       */
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
	if (final_envp_path == NULL)
		ft_error_message(NO_CMD_ERR, -1);
	return (final_envp_path);
}

static void	process_last_cmd(t_data *data, t_cmd *cmd_node, int input_fd)
{
	int output_fd;
	
	input_fd = update_input_fd(cmd_node, input_fd);
	output_fd = get_output_fd(cmd_node, NULL);
	duplicate_fds(input_fd, output_fd);
	if (input_fd > 0)
		close(input_fd);
	if (output_fd > 2)
		close(output_fd);
	if (access(cmd_node->cmd[0], X_OK) != 0)
		cmd_node->cmd[0] = find_cmd_path(data->envp, cmd_node->cmd[0]);
	if (cmd_node->cmd[0] != NULL && output_fd > 0 && cmd_node->redir_error == false)
	{
		if (execve(cmd_node->cmd[0], cmd_node->cmd, data->envp_arr) < 0)
			perror("execve failed");
	}
}

static void	process_cmd(t_data *data, t_cmd *cmd_node, int input_fd, int *fd_pipe)
{
	int output_fd;

	input_fd = update_input_fd(cmd_node, input_fd);
	if (input_fd < 0)
	{
		close(fd_pipe[0]);
		close(fd_pipe[1]);
		exit(1);
	}
	output_fd = get_output_fd(cmd_node, fd_pipe);
	duplicate_fds(input_fd, output_fd);
	close(fd_pipe[0]);
	close(fd_pipe[1]);
	if (input_fd > 0)
		close(input_fd);
	if (access(cmd_node->cmd[0], X_OK) != 0)
		cmd_node->cmd[0] = find_cmd_path(data->envp, cmd_node->cmd[0]);
	if (cmd_node->cmd[0] != NULL && input_fd >= 0 && cmd_node->redir_error == false)
	{
		if (execve(cmd_node->cmd[0], cmd_node->cmd, data->envp_arr) < 0)
			perror("execve failed");
	}
	exit(1);
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
			process_last_cmd(data, cmd_node, input_fd);
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
			process_cmd(data, cmd_node, input_fd, fd_pipe);
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
	// if (data->envp_arr)
	// 	free_ft_split(data->envp_arr);
	data->envp_arr = convert_envp_llist_to_array(data->envp);
	recursive_pipeline(0, data, data->cmd);
}
