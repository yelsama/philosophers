NAME = philo

SOURCES = main.c philos_life.c main_utils.c philos_life_utils.c philos_lucked_funcs.c \
			philo_atoi.c \

OBJECTS = $(SOURCES:%.c=%.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -g

all: $(NAME)

$(NAME):$(OBJECTS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJECTS)

run: all
	./$(NAME) $$ARG

clean:
	/bin/rm -f $(OBJECTS)

fclean: clean
	/bin/rm -f $(NAME)

re: fclean all