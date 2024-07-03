/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfeliz-r <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 15:14:45 by cfeliz-r          #+#    #+#             */
/*   Updated: 2024/07/03 12:29:24 by cfeliz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	clean_up(char **args, char *path)
{
	int	i;

	i = -1;
	if (path)
		free(path);
	if (args)
	{
		while (args[++i])
			free(args[i]);
		free(args);
	}
}

void	display_argument_error(void)
{
	ft_putstr_fd("Error: Invalid arguments\n", 2);
	ft_putstr_fd("Usage:\n", 2);
	ft_putstr_fd("  ./pipex <file1> <cmd1> <cmd2> ... <file2>\n", 2);
	ft_putstr_fd("  ./pipex here_doc <LIMITER> <cmd1> <cmd2> ... <file2>\n", 2);
	exit(EXIT_FAILURE);
}

void	error(char *str)
{
	perror(str);
	exit(EXIT_FAILURE);
}

void	close_fds(int fd[2], int prev_fd)
{
	close(fd[1]);
	if (prev_fd != -1)
		close(prev_fd);
}

void	execute_absolute_path_command(char **args, t_pipex *pipex)
{
	if (execve(args[0], args, pipex->envp) == -1)
	{
		clean_up(args, NULL);
		error("ERROR: absolute route not found");
	}
}
