/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rduro-pe <rduro-pe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 12:52:43 by rduro-pe          #+#    #+#             */
/*   Updated: 2025/06/04 13:01:53 by rduro-pe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Inc/minishell.h"

/// @brief Removes Variables from Environment according to NODE
/// @param ms Overarching Minishell Structure
/// @param node Current usnet node to be executed
void	unset_built_in(t_minishell *ms, t_tree_node *node)
{
	int	env_idx;
	int	i;

	ms->exit_status = 0;
	if (!node->right)
		return ;
	if (node->right->cont.args[0][0] == '-' && node->right->cont.args[0][1])
	{
		ft_printf_fd(2, "unset: -%c: invalid option\n",
			node->right->cont.args[0][1]);
		ft_printf_fd(2, "unset: usage: unset [name ...]\n");
		ms->exit_status = 2;
		return ;
	}
	i = -1;
	while (node->right->cont.args[++i])
	{
		env_idx = get_env_idx(node->right->cont.args[i], ms->env);
		if (env_idx != -1)
			remove_env_var(ms, env_idx, ft_matrixlen(ms->env));
	}
}

/// @brief Removes variable located at IDX from the Environment
/// @param ms Overarching Minishell Structure
/// @param idx Index of the variable to be removed
/// @param len Length of the Environment
void	remove_env_var(t_minishell *ms, size_t idx, size_t len)
{
	char	**new;
	int		i;
	int		j;

	if (!ms->env || len < 1 || idx >= len)
		return ;
	new = ft_calloc(len, sizeof(char *));
	if (!new)
		return (perror("malloc"), minishell_clean(*ms, 1));
	if ((int)idx < ms->env_start)
		ms->env_start--;
	i = -1;
	j = -1;
	while (++i < (int)len)
	{
		if (i == (int)idx)
			free(ms->env[i++]);
		new[++j] = ms->env[i];
	}
	free(ms->env);
	ms->env = new;
}
