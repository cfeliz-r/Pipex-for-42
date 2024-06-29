/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_and_path.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfeliz-r <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 16:27:25 by cfeliz-r          #+#    #+#             */
/*   Updated: 2024/06/29 22:39:40 by cfeliz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	*join_paths(char *dir, char *cmd)
{
	char	*full_path;
	char	*result;

	full_path = ft_strjoin(dir, "/");
	result = ft_strjoin(full_path, cmd);
	clean_up(NULL, full_path);
	return (result);
}

static char	**get_paths(char **envp)
{
	int	i;

	i = 0;
	while (envp[i] && ft_strnstr(envp[i], "PATH", 4) == 0)
		i++;
	if (!envp[i])
		return (NULL);
	return (ft_split(envp[i] + 5, ':'));
}

static char	*search_paths(char **paths, char *cmd)
{
	char	*res;
	int		i;

	i = 0;
	while (paths[i])
	{
		res = join_paths(paths[i], cmd);
		if (access(res, F_OK) == 0)
		{
			clean_up(paths, NULL);
			return (res);
		}
		free(res);
		i++;
	}
	return (NULL);
}

char	*find_command_path(char *cmd, char **envp)
{
	char	**paths;
	char	*res;

	if (cmd[0] == '/')
	{
		if (access(cmd, F_OK | X_OK) == 0)
			return (ft_strdup(cmd));
		else
			return (NULL);
	}
	paths = get_paths(envp);
	if (paths == NULL)
		return (NULL);
	res = search_paths(paths, cmd);
	if (res == NULL)
		clean_up(paths, NULL);
	return (res);
}
