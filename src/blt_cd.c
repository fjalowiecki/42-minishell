/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgrabows <fgrabows@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 17:18:15 by fgrabows          #+#    #+#             */
/*   Updated: 2024/10/14 20:45:37 by fgrabows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//implementuje 
//co jezeli nie ma pwd i oldpwd
//co jezeli usuniesz plik w ktorym jestes
//implementacja kropek (. | ..)
//jesli cos nie tak to sie nie przesuwamy 
void cd_bltin (char **cmd, t_data *data)
{
	if (cmd[1] && cmd[2])
		printf("%s: Too many arguments", cmd[0]);
	else if(!cmd[1])
		;//cheange pwd for home and Oldpwd for pwd
	else
		cd_handler(cmd[1], data);
	
	
}

void cd_handler(char *str, t_data *data)
{
	int i;
	int *pos;

	i = 0;
	if (!ft_strchr(str,'\\'))
		
	while(ft_strchr);
}