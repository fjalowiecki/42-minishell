/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgrabows <fgrabows@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 12:27:09 by fjalowie          #+#    #+#             */
/*   Updated: 2024/10/17 15:40:10 by fgrabows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void init(t_data *data, char **envp)
{
	data->envp = fetch_envp(envp);
	increment_shlvl(data->envp);
	data->envp_arr = NULL;
	data->line = NULL;
	data->cmd_exit_status = 0;
}

void free_resources(t_data *data)
{
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
	if (data->cmd->next != NULL)
		return ;
	if (ft_strncmp(data->cmd->cmd[0], "exit", 5) == 0)
		exit_bltin(data);
	else if (ft_strncmp(data->cmd->cmd[0], "export", ft_strlen(data->cmd->cmd[0])) == 0)
		export_bltin(data->cmd->cmd, data);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;
	char *san_line;
	t_token *tokens;//to zmienie w nastepnym pushu -FG
	t_cmd *cmds;

	if (argc != 1 || envp == NULL || *envp == NULL)
	{
		perror(NO_ENVP_ERR);
		return (1);
	}
	init(&data, envp);
	while (1)
	{
		data.line = readline("\033[1;36mminishell> \033[0m");
		if (!data.line)
		{
			perror("Readline failed");
			break;
		}
		if (check_syntax(data.line))
		{
			free(data.line);
			continue;
		}
		san_line = sanitaze_line(data.line);
		if (!san_line)
		{
			free(data.line);
			free_resources(&data);
			return (-1);
		}
		//printf("input: %s\n", san_line);
		tokens = ft_tokenizer(san_line, data.envp);
		if (tokens == NULL)
			continue;
		//ft_print_token_types(tokens);
		data.cmd = ft_commands(tokens);
		if (data.cmd == NULL)
			continue;
		// ft_print_commands(cmds);
		free(data.line);
		data.line = NULL;
		// printf("cmd: %s\n", data.cmd->cmd[0]);
		check_for_builtins(&data);
		execute_cmds(&data);
		ft_free_commands(&(data.cmd));
	}
	free_resources(&data);
}
