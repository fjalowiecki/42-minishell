/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgrabows <fgrabows@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 09:53:27 by fgrabows          #+#    #+#             */
/*   Updated: 2024/10/17 15:37:52 by fgrabows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void ft_print_env_var(t_data *data);
static int ft_change_env(char *var, char *value, int j, t_data *data);

//niezaleznie na ktorym miejscu jest argument z = export dziala
//argument bez '=' tez powstaje 
//bez argumentow wyswietla env 
//zwraca 1 jezeli nazwa zmiennej jest niepoprawna
//po kolei sprawdza argumenty i dopoki nie na trafi na bledna to tworzy

void export_bltin(char **cmd, t_data *data)
{
	int i;
	int j;

	i = 1;
	j = 0;
	if(!cmd[i])
		ft_print_env_var(data);
	while (cmd[i])
	{
		if (!ft_isalpha(cmd[i][j]) && cmd[i][j] != '_')
			printf("Export: not a valid identifier");
		j++;
		while (cmd[i][j] && cmd[i][j] != '=')
		{
			if (!ft_isalnum(cmd[i][j]) && cmd[i][j] != '_')
				;//exit (1);
			j++;
		}
		if(ft_change_env(cmd[i],&cmd[i][j], j , data ) == -1)
			;//exit(1);
		i++;
		j = 0;
	}
	;//exit (0);
	
}

int ft_change_env(char *var, char *value, int j, t_data *data)
{
	printf("i got here\n");
	t_envp *node;
	char *var_name;
	char *dup_var;
	var_name = ft_substr(var, 0, j);
	if (!var_name)
		return(ft_perror_message());
	dup_var = ft_strdup(var);
	if (!dup_var)
		return(ft_perror_message());
	node = fetch_envp_node(data->envp, var_name);
	free(var_name);
	if(!node)
	{
		append_envp_node(data->envp, dup_var);//DOPISAC ERROR
		return(0);
	}
	free(node->value);
	node->value = dup_var;
	return(0);
}
static void ft_print_env_var(t_data *data)
{
	t_envp *envp;

	envp = data->envp;
	while(envp)
	{
		printf("declare -x %s\n", envp->value);
		envp = envp->next;
	}
	exit(0);
}
