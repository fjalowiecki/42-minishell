/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgrabows <fgrabows@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 09:53:27 by fgrabows          #+#    #+#             */
/*   Updated: 2024/10/19 15:19:26 by fgrabows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int ft_change_env(char *var, int j, t_data *data);

//niezaleznie na ktorym miejscu jest argument z = export dziala
//argument bez '=' tez powstaje 
//bez argumentow wyswietla env 
//zwraca 1 jezeli nazwa zmiennej jest niepoprawna
//po kolei sprawdza argumenty i dopoki nie na trafi na bledna to tworzy

int export_bltin(char **cmd, t_data *data)
{
	int i;
	int j;

	i = 1;
	j = 0;
	if(data->cmd->next || !cmd[1])
		return (1);
	while (cmd[i])
	{
		if (!ft_isalpha(cmd[i][j]) && cmd[i][j] != '_')
		{
			printf("Export: not a valid identifier\n");
			return (1);
		}
		j++;
		while (cmd[i][j] && cmd[i][j] != '=')
		{
			if (!ft_isalnum(cmd[i][j]) && cmd[i][j] != '_')
				return (1);
			j++;
		}
		if(ft_change_env(cmd[i], j , data ) == -1)
			return (1);
		i++;
		j = 0;
	}
	return (0);
}

static int ft_change_env(char *var, int j, t_data *data)
{
	t_envp *node;
	char *var_name;
	char *dup_var;

	var_name = ft_substr(var, 0, j);
	if (!var_name)
		return(ft_perror_message());
	if(!var[j])
		dup_var = ft_strjoin(var,"=");
	else	
		dup_var = ft_strdup(var);
	if (!dup_var)
		return(ft_perror_message());
	node = fetch_envp_node(data->envp, var_name);
	free(var_name);
	if(!node)
	{
		append_envp_node(&data->envp, dup_var);//DOPISAC ERROR
		return(0);
	}
	free(node->value);
	node->value = dup_var;
	return(0);
}
int ft_print_env_var(t_data *data)
{
	t_envp *envp;

	if(data->cmd->cmd[1])
		exit(data->cmd_exit_status);
	envp = data->envp;
	while(envp)
	{
		printf("declare -x %s\n", envp->value);
		envp = envp->next;
	}
	exit (0);
}
