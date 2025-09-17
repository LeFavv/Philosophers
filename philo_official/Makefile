# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vafavard <vafavard@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/26 12:48:55 by kevwang           #+#    #+#              #
#    Updated: 2025/09/17 11:02:11 by vafavard         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc

CFLAGS = -Wall -Wextra -Werror #-g -fsanitize=thread

NAME = philo

FILE =	philo.c \
		philo_init.c \
		philo_utils.c \
		fork.c \
		time.c \
		thread_and_mutex.c \
		eat.c \
		routine.c \
		print.c \
		free.c


SRC = ${FILE:.c=.o}

all : ${NAME}

${NAME}: ${SRC}
	${CC} ${CFLAGS} -o $@ $^  ${LDLAGS}


%.o:%.c
	${CC} ${CFLAGS} -c $^

clean:
	rm -f philo.o \
		philo_init.o \
		philo_utils.o \
		fork.o \
		time.o \
		thread_and_mutex.o \
		eat.o \
		routine.o \
		print.o \
		free.o

fclean: clean
	rm -f philo

re: fclean
	make