/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgrabows <fgrabows@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 09:12:05 by fjalowie          #+#    #+#             */
/*   Updated: 2024/10/16 19:26:13 by fgrabows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void exit_bltin(t_data *data)
{
	free_resources(data);
	//needs signals implented to work
	exit(0);
}

void env_bltin(t_data *data)
{
	t_envp *envp;

	if(data->cmd->cmd[1])
	{
		printf("env: Too many arguments");
		exit(1);
	}
	envp = data->envp;
	while(envp)
	{
		printf("%s\n", envp->value);
		envp = envp->next;
	}
	exit(0);
}

// void echo_bltin(char **cmd, t_data *data)
// {
// 	int add_nl;

// 	add_nl = 1;
// 	cmd++;
// 	while (ft_strncmp(*cmd, "-n", 3) == 0)
// 	{
// 		add_nl = 0;
// 		cmd++;
// 	}
// 	while (*cmd != NULL)
// 	{
// 		printf("%s", *cmd);
// 		if (add_nl && (*(cmd+1) == NULL))
// 			printf("\n");
// 		if (*(cmd+1) != NULL)
// 			printf(" ");
// 		cmd++;
// 	}
// 	exit(0);
// }

int check_for_builtin_and_execute(char **cmd, t_data *data)
{
	// char **cmd2 = cmd;

	// while (*cmd2 != NULL)
	// {
	// 	fprintf(stderr, "cmd: %s\n", *cmd2);
	// 	cmd2++;
	// }

	if (ft_strncmp(cmd[0], "exit", ft_strlen(cmd[0])) == 0)
		exit_bltin(data);
	else if (ft_strncmp(cmd[0], "env", ft_strlen(cmd[0])) == 0)
		env_bltin(data);
	else if (ft_strncmp(cmd[0], "echo", ft_strlen(cmd[0])) == 0)
		echo_bltin(cmd, data);
	else if (ft_strncmp(cmd[0], "pwd", ft_strlen(cmd[0])) == 0)
		pwd_bltin(cmd, data);
	else if (ft_strncmp(cmd[0], "cd", ft_strlen(cmd[0])) == 0)
		exit (0);
	else if (ft_strncmp(cmd[0], "unset", ft_strlen(cmd[0])) == 0)
		;
	else if (ft_strncmp(cmd[0], "export", ft_strlen(cmd[0])) == 0)
		;
	return(0);
}
