/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_pwd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgrabows <fgrabows@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 19:52:20 by fgrabows          #+#    #+#             */
/*   Updated: 2024/10/24 09:10:00 by fgrabows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pwd_bltin(void)
{
	char	path[4096];

	if (!(getcwd(path, 4096)))
	{
		ft_perror_message();
		exit(1);
	}
	else
		printf("%s\n", path);
	exit(0);
}
