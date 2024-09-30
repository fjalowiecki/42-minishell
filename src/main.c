/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 12:27:09 by fjalowie          #+#    #+#             */
/*   Updated: 2024/09/30 14:27:49 by fjalowie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void init(t_data *data, char **envp)
{
	data->envp = fetch_envp(envp);
	increment_shlvl(data->envp);
	data->envp_arr = NULL;
	data->cmd_exit_status = 0;
	data->redirs = NULL;
	data->cmd = NULL;
	
	// QUICK TEST (TEMP)
/* 	data->redirs = malloc(sizeof(t_redirs));
	// data->redirs->infile = strdup("input");
	data->redirs->infile = NULL;
	// data->redirs->outfile = strdup("output2.txt");
	data->redirs->outfile = NULL;
	data->redirs->append = false;
	t_cmd *cmd2 = malloc(sizeof(t_cmd));
	cmd2->cmd = malloc(2 * sizeof(char *));
	cmd2->cmd[0] = strdup("cat");
	// cmd2->cmd[1] = strdup("abc");
	cmd2->cmd[1] = NULL;
	cmd2->next = NULL;
	data->cmd = malloc(sizeof(t_cmd));
	data->cmd->cmd = malloc(3 * sizeof(char *));
	data->cmd->cmd[0] = strdup("echo");
	data->cmd->cmd[1] = strdup("abc");
	data->cmd->cmd[2] = NULL;
	data->cmd->next = cmd2; */
}

void free_resources(t_data *data)
{
	free_shlvl_value(data->envp);
	free_envp(data->envp);
	if (data->envp_arr)
		free(data->envp_arr);

/* 	free(data->redirs->outfile);
	free(data->redirs);
	free_ft_split(data->cmd->next->cmd);
	free(data->cmd->next);
	free_ft_split(data->cmd->cmd);
	free(data->cmd); */
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;
	char *san_line;
	t_token *tokens;

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
			perror("readline failed");
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
		printf("input: %s\n", san_line);
		tokens = ft_tokenizer(san_line);
		if (tokens == NULL)
			continue;
		execute_cmds(&data);
		free(data.line);
	}
	free_resources(&data);
}
