/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfeliz-r <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 15:14:45 by cfeliz-r          #+#    #+#             */
/*   Updated: 2024/06/28 16:34:21 by cfeliz-r         ###   ########.fr       */
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
	ft_putstr_fd("Error: Bad arguments\n", 2);
	ft_putstr_fd("Use: ./pipex <file> <cmd1> <cmd2> ... <file>\n", 1);
	ft_putstr_fd("Use:./pipex <here_doc> <Limiter> <cmd1> <cmd2>..<file>\n", 1);
	exit(2);
}

void	error(char *str)
{
	ft_putstr(str);
	exit(EXIT_FAILURE);
}

void	close_fds(int fd[2], int prev_fd)
{
	close(fd[1]);
	if (prev_fd != -1)
		close(prev_fd);
}
