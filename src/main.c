/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 12:27:09 by fjalowie          #+#    #+#             */
/*   Updated: 2024/09/20 15:02:35 by fjalowie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void init(t_data *data, char **envp)
{
	data->envp = fetch_envp(envp);
}

void free_resources(t_data *data)
{
	free_envp(data->envp);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;
	char	*line;

	if (argc != 1 || envp == NULL || *envp == NULL)
	{
		printf("Error: no environment found\n");
		return (1);
	}
	init(&data, envp);
	while (1)
	{
		line = readline("minishell> ");
		printf("your input: %s", line);
		free(line);
	}
	free_resources(&data);
}
