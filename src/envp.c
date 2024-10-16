/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 13:37:12 by fjalowie          #+#    #+#             */
/*   Updated: 2024/10/16 12:40:53 by fjalowie         ###   ########.fr       */
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
		free(tmp);
	}
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
		if (!envp_node)
		{
			free_envp(envp_head);
			return (NULL);
		}
		envp_node->value = strdup(*envp);
		if (!envp_node->value)
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

void	append_envp_node(t_envp *head, char *str)
{
	t_envp	*node;
	t_envp	*new_node;

	node = head;
	while (node->next != NULL)
		node = node->next;
	new_node = malloc(sizeof(t_envp));
	if (!new_node)
	{
		perror("append_envp_node");
		return ;
	}
	new_node->next = NULL;
	new_node->value = str;
	node->next = new_node;
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
	while (node->next != NULL)
	{
		if (ft_strncmp(key, node->value, key_len) == 0)
			return (node);
		node = node->next;
	}
	return (NULL);
}

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

char **convert_envp_llist_to_array(t_envp *head)
{
	t_envp *node;
	size_t nodes_cnt;
	size_t i;
	char **arr;

	nodes_cnt = 0;
	node = head;
	while (node)
	{
		nodes_cnt++;
		node = node->next;
	}
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
