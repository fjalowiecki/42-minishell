/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_echo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgrabows <fgrabows@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 18:48:05 by fgrabows          #+#    #+#             */
/*   Updated: 2024/10/14 17:17:39 by fgrabows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void ft_echo_args(char **cmd, int fd);

//return error if a write error occurs - need to implement
//write to pipe or write to file or std???
void echo_bltin(char **cmd, t_data *data)
{
	int i;
	int j;
	bool flag;

	i = 1;
	j = 0;
	flag = false;
	while(cmd[i])
	{
		if(!cmd[i][j] || cmd[i][j] != '-')
			break;
		j++;
		while(cmd[i][j] && cmd[i][j] == 'n')
			j++;
		if(cmd[i][j] || j == 1)
			break;
		j = 0;
		flag = true;
		i++;
	}
	ft_echo_args(&cmd[i], 1);
	if (flag == false)
		ft_putchar_fd('\n', 1);//just to be sure
	exit(0);
}
static void ft_echo_args(char **cmd, int fd)
{
	int i;

	i = 0;
	while (cmd[i])
	{
		ft_putstr_fd(cmd[i], fd);
		i++;
		if (cmd[i])
			ft_putchar_fd(' ', fd);
		else
			break;
	}
}