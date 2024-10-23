/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp01.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 17:37:11 by fjalowie          #+#    #+#             */
/*   Updated: 2024/10/23 17:52:36 by fjalowie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	increment_shlvl(t_envp *head)
{
	t_envp	*node;
	char	*shlvl;
	int		shlvl_nb;

	node = fetch_envp_node(head, "SHLVL");
	shlvl = ft_strchr(node->value, '=') + 1;
	shlvl_nb = ft_atoi(shlvl);
	shlvl_nb++;
	shlvl = ft_itoa(shlvl_nb);
	if (!shlvl)
		perror("ft_itoa");
	free(node->value);
	node->value = ft_strjoin("SHLVL=", shlvl);
	if (!node->value)
		perror("ft_strjoin");
	free(shlvl);
}

void	free_shlvl_value(t_envp *head)
{
	t_envp	*node;

	node = fetch_envp_node(head, "SHLVL");
	free(node->value);
	node->value = NULL;
}

char	**convert_envp_llist_to_array(t_envp *head)
{
	t_envp	*node;
	size_t	nodes_cnt;
	size_t	i;
	char	**arr;

	nodes_cnt = 0;
	node = head;
	while (node && ++nodes_cnt)
		node = node->next;
	arr = malloc(sizeof(char *) * (nodes_cnt + 1));
	if (!arr)
		return (NULL);
	node = head;
	i = 0;
	while (node)
	{
		arr[i] = ft_strdup(node->value);
		if (!arr[i])
			return (NULL);
		i++;
		node = node->next;
	}
	arr[i] = NULL;
	return (arr);
}

t_envp	*fetch_envp(char **envp)
{
	t_envp	*envp_node;
	t_envp	*envp_head;
	t_envp	*envp_node_prev;

	envp_node = NULL;
	envp_head = NULL;
	envp_node_prev = NULL;
	while (*envp != NULL)
	{
		envp_node = malloc(sizeof(t_envp));
		envp_node->value = strdup(*envp);
		if (!envp_node || !envp_node->value)
		{
			free_envp(envp_head);
			return (NULL);
		}
		envp_node->next = NULL;
		if (envp_node_prev != NULL)
			envp_node_prev->next = envp_node;
		else
			envp_head = envp_node;
		envp_node_prev = envp_node;
		envp++;
	}
	return (envp_head);
}
