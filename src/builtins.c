/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgrabows <fgrabows@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 09:12:05 by fjalowie          #+#    #+#             */
/*   Updated: 2024/10/13 19:56:03 by fgrabows         ###   ########.fr       */
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
	else if (ft_strncmp(cmd[0], "echo", ft_strlen(cmd[0])) == 0)
		echo_bltin(cmd, data);
	else if (ft_strncmp(cmd[0], "cd", 2) == 0)
		;
	else if (ft_strncmp(cmd[0], "pwd", 3) == 0)
		;
	else if (ft_strncmp(cmd[0], "env", 3) == 0)
		;
}
