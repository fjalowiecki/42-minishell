/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 19:06:02 by fjalowie          #+#    #+#             */
/*   Updated: 2024/10/24 12:02:57 by fjalowie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_bltin(t_data *data)
{
	int	exit_status;
	int	i;

	if (data->cmd->cmd[1] != NULL)
	{
		i = 0;
		while (data->cmd->cmd[1][i] != '\0')
		{
			if (!ft_isdigit(data->cmd->cmd[1][i])
				&& !(i == 0 && data->cmd->cmd[1][i] == '-'))
			{
				msg_error(NUM_REQ_ERR);
				free_resources(data);
				exit(2);
			}
			i++;
		}
		exit_status = ft_atoi(data->cmd->cmd[1]);
	}
	else
		exit_status = data->cmd_exit_status;
	free_resources(data);
	exit(exit_status);
}

void	env_bltin(t_data *data)
{
	t_envp	*envp;

	if (data->cmd->cmd[1])
	{
		ft_error_message("env: Too many arguments", 2);
		exit(1);
	}
	envp = data->envp;
	while (envp)
	{
		printf("%s\n", envp->value);
		envp = envp->next;
	}
	exit(0);
}

int	check_for_builtin_and_execute(char **cmd, t_data *data)
{
	if (!cmd || !cmd[0])
		return (-1);
	if (ft_strncmp(cmd[0], "exit", ft_strlen(cmd[0])) == 0)
		exit_bltin(data);
	else if (ft_strncmp(cmd[0], "env", ft_strlen(cmd[0])) == 0)
		env_bltin(data);
	else if (ft_strncmp(cmd[0], "echo", ft_strlen(cmd[0])) == 0)
		echo_bltin(cmd);
	else if (ft_strncmp(cmd[0], "pwd", ft_strlen(cmd[0])) == 0)
		pwd_bltin();
	else if (ft_strncmp(cmd[0], "cd", ft_strlen(cmd[0])) == 0)
		exit (data->cmd_exit_status);
	else if (ft_strncmp(cmd[0], "unset", ft_strlen(cmd[0])) == 0)
		exit(data->cmd_exit_status);
	else if (ft_strncmp(cmd[0], "export", ft_strlen(cmd[0])) == 0)
		exit(ft_print_env_var(data));
	return (0);
}
