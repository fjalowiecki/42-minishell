/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgrabows <fgrabows@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 17:18:15 by fgrabows          #+#    #+#             */
/*   Updated: 2024/10/20 00:25:45 by fgrabows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int cd_handler(char *str, t_data *data);
static int ft_change_value(char *var, char *res, t_data *data);
static int ft_cd_env_change(t_data *data, char *var);

//implementuje 
//co jezeli nie ma pwd i oldpwd
//co jezeli usuniesz plik w ktorym jestes
//implementacja kropek (. | ..)
//jesli cos nie tak to sie nie przesuwamy 
//cd musi zmienic w rodzicu
//trzeba sie zajac jeszcze wyjsciem z programu
//bład 1
//wszystko ok 0

int cd_bltin(char **cmd, t_data *data)
{
	char *path;
	char curr[4096];
	
	if (cmd[1] && cmd[2])
	{
		printf("%s: Too many arguments", cmd[0]);
		return(1);
	}
	else if(!cmd[1])
	{
		path = getenv("HOME");
		if (!path)
		{
			printf("HOME not set\n");
			return(1);
		}
		if(chdir(path))
		{
			ft_perror_message();
			return (1);
		}
		getcwd(curr, 4096);
		if(ft_change_value("OLDPWD", getenv("PWD"), data) == -1)
			return(-1);
		if(ft_change_value("PWD", curr, data) == -1)
			return(-1);
	}
	else if (cd_handler(cmd[1], data) == -1)
		return(-1);
	return(0);
}

static int ft_change_value(char *var, char *res, t_data *data)
{
	t_envp *node;
	char *str;
	int var_len;
	int res_len;
	
	if (!res)
		return(ft_cd_env_change(data, var));
	var_len = ft_strlen(var);
	res_len = ft_strlen(res);
	node = fetch_envp_node(data->envp, var);
	str = malloc(sizeof(char) * (var_len + res_len + 2));
	if (!str)
		return(ft_perror_message());
	ft_strlcpy(str, var, var_len + 1);
	str[var_len] = '=';
	ft_strlcpy(&str[var_len + 1], res, res_len + 1);
	if(!node)
	{
		if(append_envp_node(&data->envp, str) == -1);
			{
				free(str);
				return (-1);
			}
		return(0);
	}
	free(node->value);
	node->value = str;
	return (0);
}

static int cd_handler(char *str, t_data *data)
{
	int value;
	char cur[4096];

	if (chdir(str) == -1)
	{
		ft_perror_message();
		return(-1);
	}
	if (ft_change_value("OLDPWD", getenv("PWD"), data) == -1)
		return (-1);
	getcwd(cur, 4096);
	if (ft_change_value("PWD", cur, data) == -1)
		return (-1);
	return(0);
}
static int ft_cd_env_change(t_data *data, char *var)
{
	t_envp *node;

	node = fetch_envp_node(data->envp, var);
	if (!node)
		return (0);
	if(data->envp == node)
		ft_remove_head_node(&data->envp);
	else
		remove_envp_node(&data->envp, fetch_node_before(&data->envp, var));
	return (0);
}
