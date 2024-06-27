/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfeliz-r <cfeliz-r@student.your42network.  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 19:23:53 by cfeliz-r          #+#    #+#             */
/*   Updated: 2024/06/27 20:20:05 by cfeliz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	execute_command_with_path(char *command, t_pipex *pipex)
{
	char	**args;
	char	*path;

	args = ft_split(command, ' ');
	path = find_command_path(args[0], pipex->envp);
	if (path == NULL)
	{
		perror("PATH Not found");
		cleanup(args, NULL, NULL);
		exit(EXIT_FAILURE);
	}
	if (execve(path, args, pipex->envp) == -1)
	{
		perror("Error in execve");
		cleanup(args, path, NULL);
		exit(EXIT_FAILURE);
	}
}

static void	handle_child_process(char *command, t_pipex *pipex)
{
	if (pipex->cmd_index == 0)
		dup2(pipex->infile, STDIN_FILENO);
	else
		dup2(pipex->prev_fd, STDIN_FILENO);
	if (pipex->cmd_index == pipex->cmd_count - 1)
		dup2(pipex->outfile, STDOUT_FILENO);
	else
		dup2(pipex->fd[1], STDOUT_FILENO);
	close(pipex->fd[0]);
	execute_command_with_path(command, pipex);
}

static void	process_commands(t_pipex *pipex)
{
	pid_t	pid;

	while (pipex->cmd_index < pipex->cmd_count)
	{
		if (pipe(pipex->fd) == -1)
			error("ERROR: failed pipe\n");
		pid = fork();
		if (pid == -1)
			error("ERROR: failed fork\n");
		if (pid == 0)
			handle_child_process(pipex->commands[pipex->cmd_index], pipex);
		else
		{
			waitpid(pid, NULL, 0);
			close_fds(pipex->fd, pipex->prev_fd);
			pipex->prev_fd = pipex->fd[0];
			pipex->cmd_index++;
		}
	}
}

int	main(int argc, char **argv, char **envp)
{
	int		infile;
	int		outfile;
	t_pipex	pipex;

	if (!*envp)
		error("Error: Environment variable is not set!\n");
	if (argc < 5)
		display_argument_error();
	infile = open(argv[1], O_RDONLY);
	if (infile == -1)
		error("Open infile");
	outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (outfile == -1)
		error("Open outfile");
	pipex.infile = infile;
	pipex.outfile = outfile;
	pipex.prev_fd = -1;
	pipex.cmd_count = argc - 3;
	pipex.commands = &argv[2];
	pipex.envp = envp;
	pipex.cmd_index = 0;
	process_commands(&pipex);
	close(infile);
	close(outfile);
	return (0);
}
