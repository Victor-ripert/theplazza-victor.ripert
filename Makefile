##
## EPITECH PROJECT, 2026
## plazza
## File description:
## Makefile
##

NAME        =   plazza

CXX         =   g++

SRC         =   src/main.cpp \
				src/kitchen/Kitchen.cpp \
				src/kitchen/Stock.cpp \
				src/kitchen/ThreadPool.cpp \
        src/reception/Parser.cpp\
			  src/reception/Reception.cpp\
			  src/errors/LowError.cpp\
			  src/errors/HighError.cpp

OBJ         =   $(SRC:.cpp=.o)

CXXFLAGS    =   -Wall -Wextra -Wno-deprecated-declarations -I./include
DEBUGFLAGS  =   -g3 -fsanitize=address
THREADFLAGS =   -lpthread

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) -o $(NAME) $(OBJ) $(CXXFLAGS) $(THREADFLAGS)

debug: CXXFLAGS += $(DEBUGFLAGS)
debug: re

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all debug clean fclean re