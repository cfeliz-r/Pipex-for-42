/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfeliz-r <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 13:52:55 by cfeliz-r          #+#    #+#             */
/*   Updated: 2024/06/29 13:14:05 by cfeliz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	create_here_doc_pipe(int here_doc_pipe[2])
{
	if (pipe(here_doc_pipe) == -1)
		error("ERROR: failed pipe\n");
}

static void	read_from_stdin_and_write_to_pipe(char **argv, int here_doc_pipe[2])
{
	char	*line;

	while (1)
	{
		ft_putstr_fd("heredoc> ", 1);
		line = get_next_line(STDIN_FILENO);
		if (ft_strncmp(line, argv[2], ft_strlen(argv[2])) == 0
			&& line[ft_strlen(argv[2])] == '\n')
			break ;
		ft_putstr_fd(line, here_doc_pipe[1]);
		free(line);
	}
	free(line);
	close(here_doc_pipe[1]);
}

static int	open_outfile(char **argv, int argc)
{
	int		outfile;

	outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0777);
	if (outfile == -1)
		error("Open outfile");
	return (outfile);
}

void	handle_here_doc(int argc, char **argv, char **envp)
{
	int		here_doc_pipe[2];
	int		outfile;
	t_pipex	pipex;

	if (argc < 6)
		display_argument_error();
	create_here_doc_pipe(here_doc_pipe);
	read_from_stdin_and_write_to_pipe(argv, here_doc_pipe);
	outfile = open_outfile(argv, argc);
	pipex.infile = here_doc_pipe[0];
	pipex.outfile = outfile;
	pipex.prev_fd = -1;
	pipex.cmd_count = argc - 4;
	pipex.commands = &argv[3];
	pipex.envp = envp;
	pipex.cmd_index = 0;
	process_commands(&pipex);
	close(here_doc_pipe[0]);
	close(outfile);
}
