# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cfeliz-r <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/24 15:15:06 by cfeliz-r          #+#    #+#              #
#    Updated: 2024/06/29 21:13:50 by cfeliz-r         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS = pipex.c utils.c check_and_path.c here_doc.c
OBJS = ${SRCS:.c=.o}
NAME = pipex
LIBFT = libft.a
LIBFT_PATH = ./libft/
RM = rm -f
CFLAGS = -Wall -Wextra -Werror -g3 -fsanitize=address,undefined,leak
CC = gcc
INFILE = infile
OUTFILE = outfile

all:		${NAME}
${NAME}:	${OBJS} ${LIBFT_PATH}${LIBFT} ${INFILE} ${OUTFILE}
			@${CC} ${CFLAGS} ${OBJS} ${LIBFT_PATH}${LIBFT} -o ${NAME}

${LIBFT_PATH}${LIBFT}:	
			@make -C ${LIBFT_PATH} --silent

$(INFILE):
	@touch $(INFILE)

$(OUTFILE):
	@touch $(OUTFILE)

clean:
	@${RM} ${OBJS} ${INFILE} ${OUTFILE}
	@make clean -C ${LIBFT_PATH}
	@echo "Cleaned object files."

fclean:		clean
	@${RM} ${NAME} ${LIBFT_PATH}${LIBFT}
	@echo "Cleaned executable and library."

re:		fclean all

.PHONY: all clean fclean re
