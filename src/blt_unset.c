/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_unset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgrabows <fgrabows@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 09:51:43 by fgrabows          #+#    #+#             */
/*   Updated: 2024/10/23 13:01:53 by fgrabows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	unset_bltin(char **cmd, t_data *data)
{
	int		i;
	int		j;
	char	*name;
	t_envp	*node;
	t_envp	*previous_nod;

	i = 1;
	j = 0;
	while (cmd[i])
	{
		while (cmd[i][j] && cmd[i][j] != '=')
			j++;	
		name = ft_substr(cmd[i], 0, j);
		if (!name)
			return (ft_perror_message());
		node = fetch_envp_node(data->envp, name);
		if (!ft_strncmp(name, data->envp->value, j) && data->envp->value[j] == '=')
			ft_remove_head_node(&(data->envp));
		else if (node)
			remove_envp_node(&data->envp,fetch_node_before(&data->envp, name));
		i++;
		j = 0;
		free(name);
	}
}

void	ft_remove_head_node(t_envp **head)
{
	t_envp	*new_head;

	new_head = (*head)->next;
	free((*head)->value);
	free(*head);
	*head = new_head;
}
