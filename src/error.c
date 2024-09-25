/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgrabows <fgrabows@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 11:16:45 by fgrabows          #+#    #+#             */
/*   Updated: 2024/09/25 12:28:51 by fgrabows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_error_message(char *str, int num)
{
	ft_putstr_fd(str, 2);
	return (num);
}

int ft_perror_message()
{
	perror(NULL);
	return(-1);
}
