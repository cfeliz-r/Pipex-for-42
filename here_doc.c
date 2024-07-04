/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfeliz-r <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 13:52:55 by cfeliz-r          #+#    #+#             */
/*   Updated: 2024/07/04 03:47:25 by cfeliz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	read_from_stdin_and_write_to_pipe(char **argv, int fd[2])
{
	char	*line;

	while (1)
	{
		ft_putstr_fd("pipe heredoc> ", 1);
		line = get_next_line(STDIN_FILENO);
		if (ft_strncmp(line, argv[2], ft_strlen(argv[2])) == 0
			&& line[ft_strlen(argv[2])] == '\n')
			break ;
		ft_putstr_fd(line, fd[1]);
		clean_up(NULL, line);
	}
	clean_up(NULL, line);
	close(fd[1]);
}

static int	open_outfile(char **argv, int argc)
{
	int		outfile;

	outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (outfile == -1)
		error("ERROR: to Open outfile in here_doc");
	return (outfile);
}

void	handle_here_doc(int argc, char **argv, char **envp)
{
	int		here_doc_pipe[2];
	t_pipex	pipex;

	if (argc < 6)
		display_argument_error();
	if (pipe(here_doc_pipe) == -1)
		error("ERROR: to creat pipe in here_doc");
	read_from_stdin_and_write_to_pipe(argv, here_doc_pipe);
	pipex.outfile = open_outfile(argv, argc);
	pipex.infile = here_doc_pipe[0];
	pipex.prev_fd = -1;
	pipex.cmd_count = argc - 4;
	pipex.commands = &argv[3];
	pipex.envp = envp;
	pipex.cmd_index = 0;
	process_commands(&pipex);
	close(here_doc_pipe[0]);
}
