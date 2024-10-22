/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 12:27:09 by fjalowie          #+#    #+#             */
/*   Updated: 2024/10/22 09:50:38 by fjalowie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init(t_data *data, int argc, char **envp)
{
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

void free_resources(t_data *data)
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

void check_for_builtins(t_data *data)
{
	if (data->cmd->next != NULL || !data->cmd->cmd)
		return ;
	if (data->cmd->cmd && ft_strncmp(data->cmd->cmd[0], "exit", 5) == 0)
		exit_bltin(data);
	else if (ft_strncmp(data->cmd->cmd[0], "export", ft_strlen(data->cmd->cmd[0])) == 0)
		export_bltin(data->cmd->cmd, data);
	else if (ft_strncmp(data->cmd->cmd[0], "unset", ft_strlen(data->cmd->cmd[0])) == 0)
		unset_bltin(data->cmd->cmd, data);
	else if (ft_strncmp(data->cmd->cmd[0], "cd", ft_strlen(data->cmd->cmd[0])) == 0)
		cd_bltin(data->cmd->cmd, data);
		
}

int read_line(t_data *data)
{
	errno = 0;
	data->line = readline("\033[1;36mminishell> \033[0m");
	if (!data->line)
	{
		if (errno == 0)
		{
			printf("exit\n");
			free_resources(data);
			exit(0);
		}
		else
		{
			perror(READLINE_ERR);
			return (-1);
		}
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;
	t_token	*tokens;

	init(&data, argc, envp);
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
		// printf("input: %s\n", data.line);
		tokens = ft_tokenizer(&data);
		if (tokens == NULL)
			continue;
		//ft_print_token_types(tokens);
		ft_check_tokens(&tokens);
		if (tokens == NULL)
			continue;
		data.cmd = ft_commands(tokens);
		if (data.cmd == NULL)
			continue;
		free(data.line);
		data.line = NULL;
		//ft_print_commands(data.cmd);
		// printf("cmd: %s\n", data.cmd->cmd[0]);
		check_for_builtins(&data);
		execute_cmds(&data);
		ft_free_commands(&(data.cmd));
	}
	free_resources(&data);
}
