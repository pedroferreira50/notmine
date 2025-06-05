/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rduro-pe <rduro-pe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 12:50:46 by rduro-pe          #+#    #+#             */
/*   Updated: 2025/06/05 12:46:29 by rduro-pe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Inc/minishell.h"

/// @brief Prints the current Environment
/// @param ms Overarching Minishell Structure
/// @param node Current env node to be executed
/// @param fd File Descriptor for where to send the output
void	env_built_in(t_minishell *ms, t_tree_node *node, int fd)
{
	if (node->right)
	{
		if (node->right->cont.args[0][0] == '-' && node->right->cont.args[0][1])
		{
			ms->exit_status = 125;
			ft_printf_fd(2, "env: invalid option -- \'%c\'\n",
				node->right->cont.args[0][1]);
		}
		else
		{
			ms->exit_status = 127;
			ft_printf_fd(2, "env: \'%s\': No such file or directory\n",
				node->right->cont.args[0]);
		}
		return ;
	}
	ms->exit_status = 0;
	print_env(*ms, 0, fd);
}

/// @brief Prints either export or env according to EXPORT_FLAG
/// @param ms Overarching Minishell Structure
/// @param export_flag 1 prints export, 0 prints env
/// @param fd File Descriptor for where to send the output
void	print_env(t_minishell ms, int export_flag, int fd)
{
	int		i;
	int		j;
	char	**temp;

	i = -1;
	if (export_flag)
	{
		temp = sort_matrix(ms.env, (int)ft_matrixlen(ms.env));
		if (!temp)
			minishell_clean(ms, 1);
		while (temp[++i])
		{
			j = 0;
			ft_printf_fd(fd, "declare -x ");
			while (temp[i][j] && temp[i][j] != '=')
				ft_printf_fd(fd, "%c", temp[i][j++]);
			if (temp[i][j] == '=')
				ft_printf_fd(fd, "%c\"%s\"", temp[i][j], &temp[i][j + 1]);
			ft_printf_fd(fd, "\n");
		}
		free(temp);
	}
	else
		while (ms.env[++i + ms.env_start])
			ft_printf_fd(fd, "%s\n", ms.env[i + ms.env_start]);
}
