/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   distributer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andcarva <andcarva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 12:50:38 by rduro-pe          #+#    #+#             */
/*   Updated: 2025/06/05 15:59:31 by andcarva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Inc/minishell.h"

/// @brief Looks at the Input Tree and sends it to the
/// corresponding executer process
/// @param ms Overarching Minishell Structure
/// @param node Head of the Tree to be executed
void	master_distributer(t_minishell *ms, t_tree_node *node)
{
	if (!node)
		return ;
	if (node->type == PIPE)
		pipe_process(ms, node);
	else if (node->type == CMD)
		command_process(ms, node);
	else if (node->type == BUILT_IN)
		built_in_process(ms, node);
}

/// @brief Opens redirections and sends a node of type CMD to be executed
/// @param ms Overarching Minishell Structure
/// @param node Current node of type CMD to be executed
void	command_process(t_minishell *ms, t_tree_node *node)
{
	int	id;
	int	redir[2];

	if (single_redir_proc(ms, node, &redir[0], &redir[1]) == -1)
		return ;
	init_sigact(ms, 'I');
	id = fork();
	if (id < 0)
		return (error_msg_status("fork", &ms->exit_status, 1));
	else if (id == 0)
	{
		init_sigact(ms, 'D');
		if (!node->cont.cmd)
			minishell_clean(*ms, 0);
		cmd_parse_and_exe(*ms, node, redir);
	}
	safe_close(redir[0]);
	safe_close(redir[1]);
	process_waiting(1, &id, &ms->exit_status);
	init_sigact(ms, 'P');
}

/// @brief Parses the cmd information from NODE, dups needed fds,
/// and executes said command
/// @param ms Overarching Minishell Structure
/// @param node Current node of type CMD to be executed
/// @param redir Int array of size 2 with fds for redirections and or pipes
void	cmd_parse_and_exe(t_minishell ms, t_tree_node *node, int *redir)
{
	char	**cmd;
	char	*path;
	int		status;

	if (node->right)
		cmd = matrix_add_front(node->cont.cmd, node->right->cont.args);
	else
		cmd = matrix_add_front(node->cont.cmd, NULL);
	if (!cmd)
		minishell_clean(ms, 1);
	path = get_path(ms, cmd[0]);
	if (!path)
		return (perror("malloc"), delete_matrix(cmd, node), \
		minishell_clean(ms, 1));
	if (redir[0] > 2)
		dup2(redir[0], STDIN_FILENO);
	if (redir[1] > 2)
		dup2(redir[1], STDOUT_FILENO);
	master_close();
	execve(path, cmd, &ms.env[ms.env_start]);
	delete_matrix(cmd, node);
	status = error_code_for_exec(path);
	free(path);
	minishell_clean(ms, status);
}

/// @brief Opens redirections and sends node of type BUILT_IN to be executed
/// @param ms Overarching Minishell Structure
/// @param node Current node of type BUILT_IN to be executed
void	built_in_process(t_minishell *ms, t_tree_node *node)
{
	int	redir[2];

	if (single_redir_proc(ms, node, &redir[0], &redir[1]) == -1)
		return ;
	built_in_exe(ms, node, redir[1], false);
	safe_close(redir[0]);
	safe_close(redir[1]);
}

/// @brief Analyses NODE and sends it to the corresponding
/// built in for execution
/// @param ms Overarching Minishell Structure
/// @param node Current node of type BUILT_IN to be executed
/// @param pipe_flag Bool value to signal if the built in is being
/// executed inside a pipe
void	built_in_exe(t_minishell *ms, t_tree_node *node, int out,
		bool pipe_flag)
{
	if (!ft_strcmp("echo", node->cont.cmd))
		echo_built_in(ms, node, out);
	else if (!ft_strcmp("cd", node->cont.cmd))
		cd_built_in(ms, node);
	else if (!ft_strcmp("pwd", node->cont.cmd))
		pwd_built_in(ms, node, out);
	else if (!ft_strcmp("export", node->cont.cmd))
		export_built_in(ms, node, out);
	else if (!ft_strcmp("unset", node->cont.cmd))
		unset_built_in(ms, node);
	else if (!ft_strcmp("env", node->cont.cmd))
		env_built_in(ms, node, out);
	else if (!ft_strcmp("exit", node->cont.cmd))
		exit_built_in(ms, node, pipe_flag);
}
