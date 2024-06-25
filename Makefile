# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cfeliz-r <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/24 15:15:06 by cfeliz-r          #+#    #+#              #
#    Updated: 2024/06/25 18:16:08 by cfeliz-r         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc
CFLAGS = -Wall -Wextra -Werror -g3 -fsanitize=address
SRC = pipex.c pipex_utils.c check_and_path.c
OBJ = $(SRC:.c=.o)
EXEC = pipex
INFILE = infile
OUTFILE = outfile
LIBDIR = ./libft
LIB = libft.a

all: $(INFILE) $(OUTFILE) $(LIBDIR)/$(LIB) $(EXEC)


$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJ) -L$(LIBDIR) -lft


%.o: %.c pipex.h
	$(CC) $(CFLAGS) -c $< -o $@

$(INFILE):
	@touch $(INFILE)

$(OUTFILE):
	@touch $(OUTFILE)

$(LIBDIR)/$(LIB):
	$(MAKE) -C $(LIBDIR)
	cp $(LIBDIR)/$(LIB) .

clean:
	@rm -f $(OBJ)
	$(MAKE) -C $(LIBDIR) clean

fclean: clean
	@rm -f $(EXEC)  $(INFILE) $(OUTFILE) $(LIB)
	$(MAKE) -C $(LIBDIR) fclean

re: fclean all

.PHONY: all clean fclean re
