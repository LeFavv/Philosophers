# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vafavard <vafavard@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/26 12:48:55 by kevwang           #+#    #+#              #
#    Updated: 2025/08/17 16:25:57 by vafavard         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc

CFLAGS = -Wall -Wextra -Werror

NAME = philo

FILE =	philo.c \
		philo_init.c \
		philo_utils.c



SRC = ${FILE:.c=.o}

all : ${NAME}

${NAME}: ${SRC}
	${CC} ${CFLAGS} -o $@ $^


%.o:%.c
	${CC} ${CFLAGS} -c $^

clean:	philo.o \
		philo_init.o \
		philo_utils.o



fclean: clean
	rm -f philo

re: fclean
	make