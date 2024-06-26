/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfeliz-r <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 15:12:27 by cfeliz-r          #+#    #+#             */
/*   Updated: 2024/06/26 19:40:45 by cfeliz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	display_argument_error(void)
{
	ft_putstr_fd("Error: Bad arguments\n", 2);
	ft_putstr_fd("Usage: ./pipex <file1> <cmd1> <cmd2> <file2>\n", 1);
}

void	error(void)
{
	perror("Error");
	exit(EXIT_FAILURE);
}

static void	handle_child_process(char *input_file, char **cmd1, \
				char **envp, int *fd)
{
	int	infile;

	infile = open(input_file, O_RDONLY);
	if (infile == -1)
		error();
	dup2(fd[1], STDOUT_FILENO);
	dup2(infile, STDIN_FILENO);
	close(fd[0]);
	execute_command(cmd1, envp);
	cleanup(cmd1, NULL, NULL);
}

static void	handle_parent_process(char *output_file, char **cmd2, \
				char **envp, int *fd)
{
	int	outfile;

	outfile = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (outfile == -1)
		error();
	dup2(fd[0], STDIN_FILENO);
	dup2(outfile, STDOUT_FILENO);
	close(fd[1]);
	execute_command(cmd2, envp);
	cleanup(cmd2, NULL, NULL);
}

int	main(int argc, char **argv, char **envp)
{
	pid_t	pid;
	int		fd[2];

	if (!*envp)
	{
		perror("Error: Environment variable is not set!");
		exit(EXIT_FAILURE);
	}
	if (argc == 5)
	{
		if (pipe(fd) == -1)
			error();
		pid = fork();
		if (pid == -1)
			error();
		if (pid == 0)
			handle_child_process(argv[1], ft_split(argv[2], ' '), envp, fd);
		else
			waitpid(pid, NULL, 0);
		handle_parent_process(argv[4], ft_split(argv[3], ' '), envp, fd);
	}
	else
		display_argument_error();
	return (0);
}
