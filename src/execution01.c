/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution01.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 10:39:17 by fjalowie          #+#    #+#             */
/*   Updated: 2024/10/22 12:55:18 by fjalowie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_error_and_exit_status(t_data *data)
{
	ft_error_message(NO_CMD_ERR, -1);
	data->cmd_exit_status = 127;
}

static char *find_correct_path(char **envp_paths, char *envp_path_part)
{
	char	*final_envp_path;
	int		i;

	i = 0;
	final_envp_path = NULL;
	while (envp_paths[i] != NULL)
	{
		final_envp_path = ft_strjoin(envp_paths[i], envp_path_part);
		if (access(final_envp_path, X_OK) == 0)
			break ;
		free(final_envp_path);
		final_envp_path = NULL;
		i++;
	}
	return (final_envp_path);
}

char	*find_cmd_path(t_data *data, char *cmd)
{
	char	*final_envp_path;
	char	**envp_paths;
	char	*envp_path_part;

	while (data->envp && ft_strncmp(data->envp->value, "PATH", 4) != 0)
		data->envp = data->envp->next;
	if (data->envp == NULL)
	{
		set_error_and_exit_status(data);
		return (NULL);
	}
	envp_path_part = ft_strjoin("/", cmd);
	envp_paths = ft_split(data->envp->value + 5, ':');
	final_envp_path = find_correct_path(envp_paths, envp_path_part);
	free_ft_split(envp_paths);
	free(envp_path_part);
	if (final_envp_path == NULL)
		set_error_and_exit_status(data);
	free(cmd);
	return (final_envp_path);
}
