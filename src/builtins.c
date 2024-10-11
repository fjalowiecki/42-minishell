/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 09:12:05 by fjalowie          #+#    #+#             */
/*   Updated: 2024/10/11 09:59:20 by fjalowie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void exit_bltin(t_data *data)
{
	free_resources(data);
	exit(0);
}

void env_bltin(t_data *data)
{
	t_envp *envp;

	envp = data->envp;
	while(envp)
	{
		printf("%s\n", envp->value);
		envp = envp->next;
	}
	exit(0);
}

void check_for_builtin_and_execute(char **cmd, t_data *data)
{
	if (ft_strncmp(cmd[0], "exit", 4) == 0)
		exit_bltin(data);
	else if (ft_strncmp(cmd[0], "env", 3) == 0)
		env_bltin(data);
}
