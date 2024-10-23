/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgrabows <fgrabows@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 17:18:15 by fgrabows          #+#    #+#             */
/*   Updated: 2024/10/23 12:55:58 by fgrabows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	cd_handler(char *str, t_data *data);
static int	ft_change_value(char *var, char *res, t_data *data);
static int	ft_cd_env_change(t_data *data, char *var, char *res);
static int	ft_cd_home(char **cmd, t_data *data, t_envp *home);

int	cd_bltin(char **cmd, t_data *data)
{	
	t_envp	*node;

	if (cmd[1] && cmd[2])
		return (ft_error_message("Too many arguments", 1));
	else if (!cmd[1])
	{
		node = fetch_envp_node(data->envp, "HOME");
		return (ft_cd_home(cmd, data, node));
	}
	else if (cd_handler(cmd[1], data) == -1)
		return (1);
	return (0);
}

static int	ft_change_value(char *var, char *res, t_data *data)
{
	t_envp	*node;
	char	*str;
	int		var_len;

	ft_cd_env_change(data, var, res);
	var_len = ft_strlen(var);
	node = fetch_envp_node(data->envp, var);
	str = malloc(sizeof(char) * (var_len + ft_strlen(res) + 2));
	if (!str)
		return (ft_perror_message());
	ft_strlcpy(str, var, var_len + 1);
	str[var_len] = '=';
	ft_strlcpy(&str[var_len + 1], res, ft_strlen(res) + 1);
	if (!node)
	{
		if (append_envp_node(&data->envp, str) == -1)
			{
				free(str);
				return (-1);
			}
		return (0);
	}
	free(node->value);
	node->value = str;
	return (0);
}

static int	cd_handler(char *str, t_data *data)
{
	int		value;
	char	cur[4096];
	t_envp	*node;

	if (chdir(str) == -1)
	{
		ft_perror_message();
		return (-1);
	}
	node = fetch_envp_node(data->envp, "PWD");
	if (node)
	{
		if (ft_change_value("OLDPWD", &node->value[4], data) == -1)
			return (-1);
	}
	getcwd(cur, 4096);
	if (ft_change_value("PWD", cur, data) == -1)
		return (-1);
	return (0);
}
static int	ft_cd_env_change(t_data *data, char *var, char *res)
{
	t_envp *node;

	if (res)
		return (0);
	node = fetch_envp_node(data->envp, var);
	if (!node)
		return (0);
	if (data->envp == node)
		ft_remove_head_node(&data->envp);
	else
		remove_envp_node(&data->envp, fetch_node_before(&data->envp, var));
	return (0);
}

static int	ft_cd_home(char **cmd, t_data *data, t_envp *home)
{
	t_envp	*node_PWD;
	char	curr[4096];
	
	if (!home)
		return (ft_error_message("HOME not set", 1));
	if (chdir(&home->value[5]))
	{
		ft_perror_message();
		return (1);
	}
	getcwd(curr, 4096);
	node_PWD = fetch_envp_node(data->envp, "PWD");
	if (!home)
	{
		if (ft_change_value("OLDPWD", NULL, data) == -1)
			return (1);
	}
	else 
	{
		if (ft_change_value("OLDPWD", &node_PWD->value[4], data) == -1)
			return (1);
	}
	if (ft_change_value("PWD", curr, data) == -1)
		return (1);
}
