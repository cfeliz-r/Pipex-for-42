/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfeliz-r <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 15:12:27 by cfeliz-r          #+#    #+#             */
/*   Updated: 2024/06/25 19:29:26 by cfeliz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*void	display_argument_error(void)
{
	ft_putstr_fd("Error: Bad arguments\n", 2);
	ft_putstr_fd("Usage: ./pipex <file1> <cmd1> <cmd2> <file2>\n", 1);
}

void	error(void)
{
	perror("Error");
	exit(EXIT_FAILURE);
}

void	handle_child_process(char *input_file, char **cmd1, \
				char **envp, int *fd)
{
	int	filein;

	filein = open(input_file, O_RDONLY);
	if (filein == -1)
		error();
	dup2(fd[1], STDOUT_FILENO);
	dup2(filein, STDIN_FILENO);
	close(fd[0]);
	execute_command(cmd1, envp);
	cleanup(cmd1, NULL, NULL);
}

void	handle_parent_process(char *output_file, char **cmd2, \
				char **envp, int *fd)
{
	int	fileout;

	fileout = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fileout == -1)
		error();
	dup2(fd[0], STDIN_FILENO);
	dup2(fileout, STDOUT_FILENO);
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
}*/
#include "pipex.h"

void	display_argument_error(void)
{
	ft_putstr_fd("Error: Bad arguments\n", 2);
	ft_putstr_fd("Usage: ./pipex <file1> <cmd1> <cmd2> ... <cmdN> <file2>\n", 1);
	ft_putstr_fd("Or: ./pipex here_doc LIMITADOR <cmd1> <cmd2> ... <cmdN> <file>\n", 1);
	exit(EXIT_FAILURE);
}

void	error(void)
{
	perror("Error");
	exit(EXIT_FAILURE);
}

void	handle_process(int in_fd, int out_fd, char *cmd, char **envp)
{
	char **args;

	args = ft_split(cmd, ' ');
	if (dup2(in_fd, STDIN_FILENO) == -1 || dup2(out_fd, STDOUT_FILENO) == -1)
		error();
	close(in_fd);
	close(out_fd);
	execute_command(args, envp);
}

void	execute_commands(int argc, char **argv, char **envp)
{
	int pipes[2];
	int in_fd, out_fd, pid;
	int i;

	in_fd = open(argv[1], O_RDONLY);
	if (in_fd == -1)
		error();

	for (i = 2; i < argc - 2; i++)
	{
		if (pipe(pipes) == -1)
			error();
		pid = fork();
		if (pid == -1)
			error();
		if (pid == 0)
		{
			close(pipes[0]);
			handle_process(in_fd, pipes[1], argv[i], envp);
		}
		else
		{
			close(pipes[1]);
			close(in_fd);
			in_fd = pipes[0];
		}
	}

	out_fd = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (out_fd == -1)
		error();
	handle_process(in_fd, out_fd, argv[argc - 2], envp);
	close(in_fd);
	close(out_fd);
}

void	handle_here_doc(int argc, char **argv, char **envp)
{
	int pipes[2];
	int in_fd, out_fd, pid;
	char *line;

	if (pipe(pipes) == -1)
		error();

	pid = fork();
	if (pid == -1)
		error();

	if (pid == 0)
	{
		close(pipes[0]);
		while ((line = get_next_line(STDIN_FILENO)) && ft_strncmp(line, argv[2], ft_strlen(argv[2])))
		{
			ft_putstr_fd(line, pipes[1]);
			free(line);
		}
		free(line);
		close(pipes[1]);
		exit(EXIT_SUCCESS);
	}
	else
	{
		waitpid(pid, NULL, 0);
		close(pipes[1]);
		in_fd = pipes[0];

		for (int i = 3; argv[i + 1]; i++)
		{
			if (pipe(pipes) == -1)
				error();
			pid = fork();
			if (pid == -1)
				error();
			if (pid == 0)
			{
				close(pipes[0]);
				handle_process(in_fd, pipes[1], argv[i], envp);
			}
			else
			{
				close(pipes[1]);
				close(in_fd);
				in_fd = pipes[0];
			}
		}

		out_fd = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0777);
		if (out_fd == -1)
			error();
		handle_process(in_fd, out_fd, argv[argc - 2], envp);
		close(in_fd);
		close(out_fd);
	}
}

int	main(int argc, char **argv, char **envp)
{
	if (!*envp)
		error();
	if (argc >= 5)
	{
		if (ft_strcmp(argv[1], "here_doc") == 0)
		{
			if (argc < 6)
				display_argument_error();
			handle_here_doc(argc, argv, envp);
		}
		else
		{
			execute_commands(argc, argv, envp);
		}
	}
	else
	{
		display_argument_error();
	}
	return (0);
}

