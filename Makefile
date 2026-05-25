##
## EPITECH PROJECT, 2026
## plazza
## File description:
## Makefile
##

NAME    =   plazza

CC      =   g++

SRC     =	src/main.c

OBJ     =   $(SRC:.c=.o)

CFLAGS  =   -Wall -Wextra -Wno-deprecated-declarations
DEBUGFLAGS = -g3 -fsanitize=address -fsanitize=leak
THREADFLAGS = -lpthread

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(CFLAGS) $(THREADFLAGS)

debug: CFLAGS += $(DEBUGFLAGS)
debug: re

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all