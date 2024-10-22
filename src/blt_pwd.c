/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_pwd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgrabows <fgrabows@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 19:52:20 by fgrabows          #+#    #+#             */
/*   Updated: 2024/10/22 09:53:12 by fgrabows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void pwd_bltin()
{
	char path[4096];

	if (!(getcwd(path, 4096)))
	{
		ft_perror_message();
		exit(1);
	}
	else
		printf("%s\n", path);
	exit(0);
}