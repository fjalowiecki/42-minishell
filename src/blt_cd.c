/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgrabows <fgrabows@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 17:18:15 by fgrabows          #+#    #+#             */
/*   Updated: 2024/10/14 19:11:19 by fgrabows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//implementuje 
//co jezeli nie ma pwd i oldpwd
//co jezeli usuniesz plik w ktorym jestes
//implementacja kropek (. | ..)
void cd_bltin (char **cmd)
{
	if (cmd[1] && cmd[2])
		printf("%s: Too many arguments", cmd[0]);
	if(!cmd[1])
		//cheange pwd for home and Oldpwd for pwd
	
	
}