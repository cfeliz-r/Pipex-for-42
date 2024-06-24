/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfeliz-r <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 15:13:02 by cfeliz-r          #+#    #+#             */
/*   Updated: 2024/06/24 15:38:55 by cfeliz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <stdio.h>
# include <sys/wait.h>
# include <fcntl.h>

# include "./libft/libft.h"

char	*find_command_path(char *cmd, char **envp);
void	execute_command(char **cmd, char **envp);
void	handle_child_process(char *input_file, char **cmd1, \
				char **envp, int *fd);
void	handle_parent_process(char *output_file, char **cmd2, \
				char **envp, int *fd);
void	cleanup(char **args, char *path, char **paths);
void	error(void);
void	display_argument_error(void);
void	free_paths(char **paths);
char	*join_paths(char *dir, char *cmd);

// BONUS

void	display_usage_error(void);
void	process_child(char *argv, char **envp);
void	process_here_doc(char *limiter, int argc);
int		open_file(char *argv, int i);
int		gnl(char **line);

#endif
