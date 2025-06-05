/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andcarva <andcarva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 12:55:00 by rduro-pe          #+#    #+#             */
/*   Updated: 2025/06/05 16:40:41 by andcarva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Inc/minishell.h"

/// @brief Assigns a Type to the Node
/// @param token Node of a list
void	assign_type_token(t_token *token, bool exp)
{
	t_token	*temp;

	if (!token)
		return ;
	temp = token;
	while (temp)
	{
		if (temp->type != PIPE && temp->type < RED_IN && temp->type > RED_APP
			&& exp == true)
			assigns_types(temp, exp);
		else if (exp == false)
			assigns_types(temp, exp);
		temp = temp->next;
	}
	assigns_cmd(token, exp);
}

/// @brief Checks if the token received is a built_in
/// @param token Node of the list of tokens
void	assigns_built_in(t_token *token)
{
	if (token->cont == NULL)
		token->type = CMD;
	if (ft_strncmp("echo", token->cont, 5) == 0)
		token->type = BUILT_IN;
	else if (ft_strncmp("cd", token->cont, 3) == 0)
		token->type = BUILT_IN;
	else if (ft_strncmp("pwd", token->cont, 4) == 0)
		token->type = BUILT_IN;
	else if (ft_strncmp("export", token->cont, 8) == 0)
		token->type = BUILT_IN;
	else if (ft_strncmp("unset", token->cont, 7) == 0)
		token->type = BUILT_IN;
	else if (ft_strncmp("env", token->cont, 4) == 0)
		token->type = BUILT_IN;
	else if (ft_strncmp("exit", token->cont, 5) == 0)
		token->type = BUILT_IN;
}

/// @brief checks if the token received is PIPE or REDIR
/// @param token Node of the list of tokens
void	assigns_types(t_token *token, bool exp)
{
	if (token->cont == NULL)
		return ;
	if (exp == false)
	{
		if (ft_strncmp("|", token->cont, 2) == 0)
			token->type = PIPE;
		else if (ft_strncmp("<", token->cont, 2) == 0)
			token->type = RED_IN;
		else if (ft_strncmp(">", token->cont, 2) == 0)
			token->type = RED_OUT;
		else if (ft_strncmp("<<", token->cont, 3) == 0)
			token->type = RED_HD;
		else if (ft_strncmp(">>", token->cont, 3) == 0)
			token->type = RED_APP;
	}
	if (exp == true)
		is_limtiter_or_arg(&token);
}

/// @brief Checks if the token received CMD
/// @param head Beggining of the list of tokens
void	assigns_cmd(t_token *head, bool exp)
{
	t_token	*temp;
	bool	its_cmd;

	temp = head;
	while (temp)
	{
		its_cmd = false;
		while (temp && temp->type != PIPE)
		{
			if ((temp->type == RED_HD || is_token(temp) || temp->cont == NULL))
			{
				is_limtiter_or_arg(&temp);
				assigns_types(temp, exp);
			}
			else if (!its_cmd && !is_token(temp))
			{
				temp->type = CMD;
				assigns_built_in(temp);
				its_cmd = true;
			}
			temp = temp->next;
		}
		if (temp && temp->type == PIPE)
			temp = temp->next;
	}
}

/// @brief Assigns a name to the token, done for the debug
/// @param type Of the Node
void	assign_name(int type)
{
	if (type == 0)
		printf("Type: %s", "CMD");
	else if (type == 1)
		printf("Type: %s", "ARG");
	else if (type == 2)
		printf("Type: %s", "PIPE");
	else if (type == 3)
		printf("Type: %s", "LIM");
	else if (type == 4)
		printf("Type: %s", "RED_IN");
	else if (type == 5)
		printf("Type: %s", "RED_HD");
	else if (type == 6)
		printf("Type: %s", "RED_OUT");
	else if (type == 7)
		printf("Type: %s", "RED_APP");
	else if (type == 8)
		printf("Type: %s", "BUILT_IN");
}
