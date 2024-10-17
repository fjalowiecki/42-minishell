/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgrabows <fgrabows@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 09:53:27 by fgrabows          #+#    #+#             */
/*   Updated: 2024/10/16 19:29:19 by fgrabows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
static int ft_print_env_var(t_data *data);

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
	if(!cmd[i])
		return(ft_print_env_var(data));
	while (cmd[i])
	{
		if (!ft_isalpha(cmd[i][j]) && cmd[i][j] != '_')
			printf("Export: not a valid identifier");
		j++;
		while (cmd[i][j] && cmd[i][j] != '=')
		{
			if (!ft_isalnum(cmd[i][j]) && cmd[i][j] != '_')
				return (1);
		}
		if(ft_change_env(cmd[i][j]) == -1)
			return(1);
		i++;
		j = 0;
	}
	return (0);
	
}

static int ft_change_env(char *var, char *value, t_data *data)
{
	
}
static int ft_print_env_var(t_data *data)
{
	t_envp *envp;

	envp = data->envp;
	while(envp)
	{
		printf("declare -x %s\n", envp->value);
		envp = envp->next;
	}
	return(0);
}
