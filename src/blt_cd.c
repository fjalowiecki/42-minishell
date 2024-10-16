/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgrabows <fgrabows@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 17:18:15 by fgrabows          #+#    #+#             */
/*   Updated: 2024/10/16 14:26:41 by fgrabows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int cd_handler(char *str, t_data *data);
static int ft_change_value(char *var, char *res, t_data *data);

//implementuje 
//co jezeli nie ma pwd i oldpwd
//co jezeli usuniesz plik w ktorym jestes
//implementacja kropek (. | ..)
//jesli cos nie tak to sie nie przesuwamy 
//cd musi zmienic w rodzicu
//trzeba sie zajac jeszcze wyjsciem z programu
//bład -1
//wszystko ok 0
int cd_bltin(char **cmd, t_data *data)
{
	//ft_putstr_fd("cd execution...\n",2);
	char *path;
	char curr[4096];
	
	if (cmd[1] && cmd[2])
	{
		printf("%s: Too many arguments", cmd[0]);
		return(-1);
	}
	else if(!cmd[1])
	{
		//getcwd(curr, 4096);
		//printf("%s\n", curr);
		path = getenv("HOME");
		if (!path)
			ft_perror_message();
		if(chdir(path))
		{
			ft_perror_message();
			return (-1);
		}
		//getcwd(curr, 4096);
		//printf("%s\n", curr);
		//ft_putstr_fd(path, 2);
		//ft_putstr_fd("-path\n", 2);
		if(ft_change_value("OLDPWD", getenv("PWD"), data) == -1)
			return(-1);
		if(ft_change_value("PWD", path, data) == -1)
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
	
	res_len = ft_strlen(res);
	var_len = ft_strlen(var);
	if (!res)
		return (0);
	node = fetch_envp_node(data->envp, var);
	if(!node)
	{
		return (0);
	}
	//printf("%s\n",node->value);
	free(node->value);
	//printf("why u bully me\n");
	str = malloc(sizeof(char) * (var_len + res_len + 1));
	if (!str)
		return(ft_perror_message());
	ft_strlcpy(str, var, var_len + 1);
	str[var_len] = '=';
	ft_strlcpy(&str[var_len + 1], res, res_len + 1);
	node->value = str;
	printf("MY line:%s\n", str);
	return (0);
}

static int cd_handler(char *str, t_data *data)
{
	int i;

	i = 0;
	if (chdir(str) == -1)
	{
		ft_perror_message();
		return(-1);
	}
	if (ft_change_value("OLDPWD", getenv("PWD"), data) == -1)
		return (-1);
	if (ft_change_value("PWD", str, data) == -1)
		return (-1);
	return(0);
}
