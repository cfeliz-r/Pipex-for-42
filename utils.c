/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfeliz-r <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 15:14:45 by cfeliz-r          #+#    #+#             */
/*   Updated: 2024/06/26 19:47:06 by cfeliz-r         ###   ########.fr       */
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

void	execute_command(char **cmd, char **envp)
{
	char	*path;

	path = find_command_path(cmd[0], envp);
	if (path == NULL)
	{
		perror("PATH Not found");
		cleanup(cmd, NULL, NULL);
		exit(EXIT_FAILURE);
	}
	if (execve(path, cmd, envp) == -1)
	{
		perror("Error in execve");
		cleanup(cmd, path, NULL);
		exit(EXIT_FAILURE);
	}
}
