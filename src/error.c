/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 11:16:45 by fgrabows          #+#    #+#             */
/*   Updated: 2024/10/21 19:41:30 by fjalowie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	msg_error(char *err)
{
	write(2, err, ft_strlen(err));
	write(2, "\n", 1);
}

int	ft_error_message(char *str, int num)
{
	ft_putstr_fd(str, 2);
	ft_putstr_fd("\n", 2);
	return (num);
}

int ft_perror_message()
{
	perror(NULL);
	return(-1);
}

void file_error_msg(char *filename, char *msg)
{
	write(2, filename, ft_strlen(filename));
	write(2, ": ", 2);
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
}

int ft_perror_free(char *first, char *second, char *third)
{
	ft_perror_message();
	if(first)
		free(first);
	if(second)
		free(second);
	if(third)
		free(third);
	return (-1);
}