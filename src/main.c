/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgrabows <fgrabows@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 12:27:09 by fjalowie          #+#    #+#             */
/*   Updated: 2024/10/24 09:09:00 by fgrabows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init(t_data *data, int argc, char **argv, char **envp)
{
	(void)argv;
	if (argc != 1)
	{
		perror(MANY_ARGS_ERR);
		exit(-1);
	}
	if (envp == NULL || *envp == NULL)
	{
		perror(NO_ENVP_ERR);
		exit(-1);
	}
	data->envp = fetch_envp(envp);
	increment_shlvl(data->envp);
	data->cmd = NULL;
	data->envp_arr = NULL;
	data->line = NULL;
	data->cmd_exit_status = 0;
}

void	free_resources(t_data *data)
{
	rl_clear_history();
	if (data->cmd != NULL)
	{
		ft_free_commands(&(data->cmd));
		data->cmd = NULL;
	}
	if (data->envp_arr)
	{
		free_ft_split(data->envp_arr);
		data->envp_arr = NULL;
	}
	free_envp(data->envp);
	data->envp = NULL;
}

void	check_for_builtins(t_data *data)
{
	if (data->cmd->next != NULL || !data->cmd->cmd)
		return ;
	if (data->cmd->cmd && ft_strncmp(data->cmd->cmd[0],
			"exit", 5) == 0)
		exit_bltin(data);
	else if (ft_strncmp(data->cmd->cmd[0],
			"export", ft_strlen(data->cmd->cmd[0])) == 0)
		data->cmd_exit_status = export_bltin(data->cmd->cmd, data);
	else if (ft_strncmp(data->cmd->cmd[0],
			"unset", ft_strlen(data->cmd->cmd[0])) == 0)
		data->cmd_exit_status = unset_bltin(data->cmd->cmd, data);
	else if (ft_strncmp(data->cmd->cmd[0],
			"cd", ft_strlen(data->cmd->cmd[0])) == 0)
		data->cmd_exit_status = cd_bltin(data->cmd->cmd, data);
}

int	read_line(t_data *data)
{
	data->line = readline("minishell> ");
	if (!data->line)
	{
		printf("exit\n");
		free_resources(data);
		exit(0);
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	init(&data, argc, argv, envp);
	while (1)
	{
		handle_signals();
		if (read_line(&data))
			continue ;
		if (data.line && *data.line)
			add_history(data.line);
		if (check_syntax(data.line))
		{
			free(data.line);
			continue ;
		}
		sanitaze_line(&data);
		if (!data.line)
			continue ;
		if (ft_cmds_creation(&data) == -1)
			continue ;
		check_for_builtins(&data);
		execute_cmds(&data);
		ft_free_commands(&(data.cmd));
	}
	free_resources(&data);
}
