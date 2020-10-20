/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/13 11:23:44 by llefranc          #+#    #+#             */
/*   Updated: 2020/10/20 17:03:47 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
** If args[0] matches a builtin, launches the proper builtin and sets
** global_ret_value with the builtin return value. Returns 1 if it launched a
** builtin, 0 otherwise.
*/
int		find_builtin(char **args, char ***env)
{
	int ret;
	
	ret = 0;
	if (!args)
		return (ret);
	!ft_strcmp("echo", args[0]) && (ret = 1) ? global_ret_value = builtin_echo(args) : 0; //if args[0] match one builtin, put ret value to 0 and launch the appropriate builtin
	!ft_strcmp("cd", args[0]) && (ret = 1) ? global_ret_value = builtin_cd(args, *env) : 0;
	!ft_strcmp("pwd", args[0]) && (ret = 1) ? global_ret_value = builtin_pwd(args) : 0;
	!ft_strcmp("export", args[0]) && (ret = 1) ? global_ret_value = builtin_export(args, env) : 0;
	!ft_strcmp("unset", args[0]) && (ret = 1) ? global_ret_value = builtin_unset(args, env) : 0;
	!ft_strcmp("env", args[0]) && (ret = 1) ? global_ret_value = builtin_env(args, *env) : 0;
	!ft_strcmp("exit", args[0]) && (ret = 1) ? global_ret_value = builtin_exit(args, *env) : 0;
	return (ret);
}

// int		do_pipe(void)

int		execution(t_token *token, char ***env)
{
	int		i;
	t_token	*tmp;

	i = -1;
	tmp = token;
	while (tmp)
	{
		if (tmp->type == INPUT || tmp->type == OUTPUT || tmp->type == OUTPUT_ADD)
			if (do_redirection(tmp->type, (tmp->args)[0]))
				return (global_ret_value = FAILURE);
		tmp = tmp->next;
	}
	tmp = token;
	while (tmp)
	{
		if (tmp->type == EXEC) //there is only one token EXEC between pipes / semicolons
			if (!find_builtin(tmp->args, env))
				execve_part(tmp->args, *env);
		tmp = tmp->next;
	}
	dup2(save_stdin, STDIN_FILENO);		//restore back stdin and stdout
	dup2(save_stdout, STDOUT_FILENO);
	// ft_printf("retour fonction = %d\n", global_ret_value);
	return (SUCCESS);
}