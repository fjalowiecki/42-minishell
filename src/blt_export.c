/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgrabows <fgrabows@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 09:53:27 by fgrabows          #+#    #+#             */
/*   Updated: 2024/10/15 11:06:23 by fgrabows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//niezaleznie na ktorym miejscu jest argument z = export dziala
//argument bez '=' tez powstaje 
//bez argumentow wyswietla env 
//zwraca 1 jezeli nazwa zmiennej jest niepoprawna
//po kolei sprawdza argumenty i dopoki nie na trafi na bledna to tworzy
void export_bltin(char **cmd)
{
	int i;
	int j;

	i = 1;
	j = 0;
	if(!cmd[i])
		;//wyswietl env
	while (cmd[i])
	{
		while(cmd[i][j] && cmd[i][j] == '=')
		{
			
		}
	}
	
}
