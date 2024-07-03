/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfeliz-r <cfeliz-r@student.your42network.  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 19:23:53 by cfeliz-r          #+#    #+#             */
/*   Updated: 2024/07/03 21:54:55 by cfeliz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	execute_relative_path_command(char **args, t_pipex *pipex)
{
	char	*path;

	path = find_command_path(args[0], pipex->envp);
	if (path == NULL)
	{
		clean_up(args, NULL);
		error("ERROR: PATH or command not found");
	}
	if (execve(path, args, pipex->envp) == -1)
	{
		clean_up(args, path);
		error("ERROR: in execve");
	}
	clean_up(NULL, path);
}

static void	execute_command_with_path(char *command, t_pipex *pipex)
{
	char	**args;

	args = ft_split(command, ' ');
	if (args[0] == NULL || !pipex->envp)
		error("ERROR: command no found or is empty");
	if (args[0][0] == '/')
		execute_absolute_path_command(args, pipex);
	else
		execute_relative_path_command(args, pipex);
	clean_up(args, NULL);
}

static void	prepare_child_process(char *command, t_pipex *pipex)
{
	if (pipex->cmd_index == 0)
	{
		if(pipex->infile == -1)
			error("ERROR: infile");
		dup2(pipex->infile, STDIN_FILENO);
	}
		
	else
		dup2(pipex->prev_fd, STDIN_FILENO);
	if (pipex->cmd_index == pipex->cmd_count - 1)
		dup2(pipex->outfile, STDOUT_FILENO);
	else
		dup2(pipex->fd[1], STDOUT_FILENO);
	close(pipex->fd[0]);
	execute_command_with_path(command, pipex);
}

void	process_commands(t_pipex *pipex)
{
	pid_t	pid;

	while (pipex->cmd_index < pipex->cmd_count)
	{
		if (pipe(pipex->fd) == -1)
			error("ERROR: failed pipe");
		pid = fork();
		if (pid == -1)
			error("ERROR: failed fork");
		if (pid == 0)
			prepare_child_process(pipex->commands[pipex->cmd_index], pipex);
		else
		{
			close_fds(pipex->fd, pipex->prev_fd);
			pipex->prev_fd = pipex->fd[0];
			pipex->cmd_index++;
		}
	}
	while (waitpid(-1, NULL, 0) != -1)
		;
}

int	main(int argc, char **argv, char **envp)
{
	int		infile;
	int		outfile;
	t_pipex	pipex;

	// if (!*envp)
	// 	error("Error: Environment variable is not set!");
	if (argc < 5)
		display_argument_error();
	if (ft_strcmp(argv[1], "here_doc") == 0)
		return (handle_here_doc(argc, argv, envp), 0);
	infile = open(argv[1], O_RDONLY);
//	if (infile == -1)
//		error("ERROR: open infile");
	outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outfile == -1)
		error("ERROR: open outfile");
	pipex.infile = infile;
	pipex.outfile = outfile;
	pipex.prev_fd = -1;
	pipex.cmd_count = argc - 3;
	pipex.commands = &argv[2];
	pipex.envp = envp;
	pipex.cmd_index = 0;
	process_commands(&pipex);
	return (close(infile), close(outfile), 0);
}
