/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfeliz-r <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 15:13:02 by cfeliz-r          #+#    #+#             */
/*   Updated: 2024/06/29 12:05:59 by cfeliz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# include <unistd.h>
# include <string.h>
# include <stdio.h>
# include <sys/wait.h>
# include "libft/libft.h"

typedef struct s_pipex
{
	int		infile;
	int		outfile;
	int		fd[2];
	int		prev_fd;
	int		cmd_index;
	int		cmd_count;
	char	**envp;
	char	**commands;
}	t_pipex;

char	*find_command_path(char *cmd, char **envp);
void	error(char *str);
void	cleanup(char **args, char *path, char **paths);
void	display_argument_error(void);
void	close_fds(int fd[2], int prev_fd);
void	handle_here_doc(int argc, char **argv, char **envp);
void	process_commands(t_pipex *pipex);
void	execute_absolute_path_command(char **args, t_pipex *pipex);

#endif
