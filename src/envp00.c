/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp00.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 13:37:12 by fjalowie          #+#    #+#             */
/*   Updated: 2024/10/23 19:06:44 by fjalowie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_envp(t_envp *head)
{
	t_envp	*tmp;

	while (head != NULL)
	{
		tmp = head;
		head = head->next;
		free(tmp->value);
		free(tmp);
	}
}

t_envp	*fetch_node_before(t_envp **head, char *key)
{
	t_envp	*p_node;
	int		key_len;

	key_len = ft_strlen(key);
	if (!(ft_strncmp(key, (*head)->value, key_len))
		&& (*head)->value[key_len] == '=')
		return (*head);
	p_node = *head;
	while (p_node)
	{
		if (!p_node->next)
			return (NULL);
		else if (!p_node->next->value)
			p_node = p_node->next;
		else if (!(ft_strncmp(key, p_node->next->value, key_len))
			&& p_node->next->value[key_len] == '=')
			return (p_node);
		else
			p_node = p_node->next;
	}
	return (NULL);
}

int	append_envp_node(t_envp **head, char *str)
{
	t_envp	*node;
	t_envp	*new_node;

	node = *head;
	while (*head && node->next != NULL)
		node = node->next;
	new_node = malloc(sizeof(t_envp));
	if (!new_node)
	{
		perror("append_envp_node");
		return (-1);
	}
	new_node->next = NULL;
	new_node->value = str;
	if (node)
		node->next = new_node;
	else
		node = new_node;
	return (0);
}

void	remove_envp_node(t_envp *prev_node)
{
	t_envp	*node;

	node = prev_node->next;
	if (node->next != NULL)
		prev_node->next = node->next;
	else
		prev_node->next = NULL;
	free (node->value);
	free (node);
}

t_envp	*fetch_envp_node(t_envp *head, char *key)
{
	t_envp	*node;
	size_t	key_len;

	key_len = ft_strlen(key);
	node = head;
	while (node != NULL)
	{
		if (ft_strncmp(key, node->value, key_len) == 0
			&& node->value[key_len] == '=')
			return (node);
		node = node->next;
	}
	return (NULL);
}
