/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution01.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 10:39:17 by fjalowie          #+#    #+#             */
/*   Updated: 2024/10/22 15:21:08 by fjalowie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void set_exit_status(int *cmd_exit_status, int status)
{
	if (WEXITSTATUS(status) == true)
		*cmd_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status) == true)
		*cmd_exit_status = 128 + WTERMSIG(status);
	else
		*cmd_exit_status = WTERMSIG(status);
}

static char *find_correct_path(t_data *data, char *cmd, int flag)
{
	char	*final_envp_path;
	char	**envp_paths;
	char	*envp_path_part;
	int		i;

	envp_path_part = ft_strjoin("/", cmd);
	envp_paths = ft_split(data->envp->value + 5, ':');
	i = 0;
	final_envp_path = NULL;
	while (envp_paths[i] != NULL)
	{
		final_envp_path = ft_strjoin(envp_paths[i], envp_path_part);
		if (access(final_envp_path, flag) == 0)
			break ;
		free(final_envp_path);
		final_envp_path = NULL;
		i++;
	}
	free_ft_split(envp_paths);
	free(envp_path_part);
	return (final_envp_path);
}

static void set_status_and_mess_err(char *err, int code, int *status)
{
	*status = code;
	msg_error(err);
}

char	*find_cmd_path(t_data *data, char *cmd, int *status)
{ //zmienicc data na envp, leak przy ech0
	char	*final_envp_path;

	while (data->envp && ft_strncmp(data->envp->value, "PATH", 4) != 0)
		data->envp = data->envp->next;
	if (data->envp == NULL)
	{
		set_status_and_mess_err(NO_CMD_ERR, 127, status);
		return (NULL);
	}
	final_envp_path = find_correct_path(data, cmd, F_OK);
	if (final_envp_path == NULL)
	{
		set_status_and_mess_err(NO_CMD_ERR, 127, status);
		free(cmd);
		return (NULL);
	}
	final_envp_path = find_correct_path(data, cmd, X_OK);
	if (final_envp_path == NULL)
	{
		set_status_and_mess_err(NO_PERM_ERR, 126, status);
		free(cmd);
		return (NULL);
	}
	free(cmd);
	return (final_envp_path);
}
