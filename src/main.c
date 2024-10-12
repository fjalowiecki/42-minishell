/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgrabows <fgrabows@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 12:27:09 by fjalowie          #+#    #+#             */
/*   Updated: 2024/10/12 20:25:43 by fgrabows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void init(t_data *data, char **envp)
{
	data->envp = fetch_envp(envp);
	increment_shlvl(data->envp);
	data->envp_arr = NULL;
	data->cmd_exit_status = 0;
}

void free_resources(t_data *data)
{
	free_shlvl_value(data->envp);
	free_envp(data->envp);
	if (data->envp_arr)
		free(data->envp_arr);
	if (data->envp_arr)
		free_ft_split(data->envp_arr);
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
		data.line = readline("minishell> ");
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
		ft_print_token_types(tokens);
		cmds = ft_commands(tokens);
		if (cmds == NULL)
			continue;
		// ft_print_commands(cmds);
		data.cmd = cmds;
		free(data.line);
		execute_cmds(&data);
		ft_free_commands(&(data.cmd));
	}
	free_resources(&data);
}
