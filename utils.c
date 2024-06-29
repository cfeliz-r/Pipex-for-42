/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfeliz-r <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 15:14:45 by cfeliz-r          #+#    #+#             */
/*   Updated: 2024/06/29 13:11:10 by cfeliz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	cleanup(char **args, char *path, char **paths)
{
	int	i;

	i = -1;
	if (path)
		free(path);
	if (paths)
	{
		while (paths[++i])
			free(paths[i]);
		free(paths);
	}
	while (args[++i])
		free(args[i]);
	free(args);
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
		cleanup(args, NULL, NULL);
		error("Error en execve con ruta absoluta");
	}
}
