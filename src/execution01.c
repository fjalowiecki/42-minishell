/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution01.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 10:39:17 by fjalowie          #+#    #+#             */
/*   Updated: 2024/10/22 11:17:50 by fjalowie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_error_and_exit_status(t_data *data)
{
	ft_error_message(NO_CMD_ERR, -1);
	data->cmd_exit_status = 127;
}

char	*find_cmd_path(t_data *data, char *cmd)
{
	char	**envp_paths;
	char	*final_envp_path;
	char	*envp_path_part;
	int		i;

	while (data->envp && ft_strncmp(data->envp->value, "PATH", 4) != 0)
		data->envp = data->envp->next;
	envp_path_part = ft_strjoin("/", cmd);
	envp_paths = ft_split(data->envp->value + 5, ':');
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
		set_error_and_exit_status(data);
	free(cmd);
	return (final_envp_path);
}
