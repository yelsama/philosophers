NAME = philo_bonus

SOURCES = main_bonus.c main_bonus_utils.c philosophers_bonus.c \
			philosophers_bonus_utils.c philo_atoi.c threads_bonus.c \
			locked_funcs_bonus.c \

OBJECTS = $(SOURCES:%.c=%.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -g

all: $(NAME)

$(NAME):$(OBJECTS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJECTS)

run: all
	./$(NAME) $$ARG

st: 
	make run ARG="2 410 200 200 6"

clean:
	/bin/rm -f $(OBJECTS)

fclean: clean
	/bin/rm -f $(NAME)

re: fclean all