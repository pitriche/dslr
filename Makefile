# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: brunomartin <brunomartin@student.42.fr>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/02/07 14:21:44 by pitriche          #+#    #+#              #
#    Updated: 2021/04/22 16:13:52 by brunomartin      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

GREY	= \033[030m
RED		= \033[031m
GREEN	= \033[032m
YELLOW	= \033[033m
BLUE	= \033[034m
MAGENTA	= \033[035m
CYAN	= \033[036m
RESET	= \033[0m

# TRAIN ********************************************************************** #

NAME_TRAIN = logreg_train

SRC_FILES_TRAIN = \
main.cpp		\


SRC_DIR_TRAIN = src_train/
SRC_TRAIN := $(addprefix $(SRC_DIR_TRAIN), $(SRC_FILES_TRAIN))

OBJ_FILES_TRAIN = $(patsubst %.cpp, %.o, $(SRC_FILES_TRAIN))
OBJ_DIR_TRAIN = obj_train/
OBJ_TRAIN := $(addprefix $(OBJ_DIR_TRAIN), $(OBJ_FILES_TRAIN))

# EXECUTE ******************************************************************** #

NAME_EXECUTE = logreg_execute

SRC_FILES_EXECUTE = \
main.cpp		\


SRC_DIR_EXECUTE = src_execute/
SRC_EXECUTE := $(addprefix $(SRC_DIR_EXECUTE), $(SRC_FILES_EXECUTE))

OBJ_FILES_EXECUTE = $(patsubst %.cpp, %.o, $(SRC_FILES_EXECUTE))
OBJ_DIR_EXECUTE = obj_execute/
OBJ_EXECUTE := $(addprefix $(OBJ_DIR_EXECUTE), $(OBJ_FILES_EXECUTE))

# **************************************************************************** #

HEADERS = \
# \
Tuple.hpp		\
DataPack.hpp	\

CC = clang++

FL_OPTI = -O3 -flto
FLAGS = -Wall -Wextra -Wconversion -Wuninitialized -Wunused -std=c++11 $(FL_OPTI)
CFLAGS = -c $(FLAGS)

CINCLUDE = -I include

INCLUDE_DIR = include/
INCLUDE := $(addprefix $(INCLUDE_DIR), $(HEADERS))

all: $(NAME_TRAIN) $(NAME_EXECUTE)
	@echo "$(CYAN)DSLR ready sir !$(RESET)"

# linking (park)
$(NAME_TRAIN): $(OBJ_DIR_TRAIN) $(OBJ_TRAIN)
	@echo "$(GREEN)train objects compiled sir !$(RESET)"
	@$(CC) $(FL_OPTI) -o $(NAME_TRAIN) $(OBJ_TRAIN)
	@echo "$(GREEN)$(NAME) linked sir !$(RESET)"

$(NAME_EXECUTE): $(OBJ_DIR_EXECUTE) $(OBJ_EXECUTE)
	@echo "$(GREEN)execute objects compiled sir !$(RESET)"
	@$(CC) $(FL_OPTI) -o $(NAME_EXECUTE) $(OBJ_EXECUTE)
	@echo "$(GREEN)$(NAME) linked sir !$(RESET)"


# compiling
$(OBJ_DIR_TRAIN)%.o : $(SRC_DIR_TRAIN)%.cpp $(INCLUDE)
	@$(CC) $(CINCLUDE) $(CFLAGS) -o $@ $<
	@echo -n '.'

$(OBJ_DIR_EXECUTE)%.o : $(SRC_DIR_EXECUTE)%.cpp $(INCLUDE)
	@$(CC) $(CINCLUDE) $(CFLAGS) -o $@ $<
	@echo -n '.'


$(OBJ_DIR_TRAIN) :
	@mkdir $(OBJ_DIR_TRAIN)
	@echo "$(GREEN)Object directory created sir !$(RESET)"

$(OBJ_DIR_EXECUTE) :
	@mkdir $(OBJ_DIR_EXECUTE)
	@echo "$(GREEN)Object directory created sir !$(RESET)"


clean:
	@rm -f $(OBJ_TRAIN)
	@rm -f $(OBJ_EXECUTE)
	@echo "$(RED)Objects deleted sir !$(RESET)"
	@rm -rf $(OBJ_DIR_TRAIN)
	@rm -rf $(OBJ_DIR_EXECUTE)
	@#echo "$(RED)Object directory deleted sir !$(RESET)"

fclean: clean
	@rm -f $(NAME_TRAIN)
	@rm -f $(NAME_EXECUTE)
	@echo "$(RED)$(NAME_TRAIN) deleted sir !$(RESET)"
	@echo "$(RED)$(NAME_EXECUTE) deleted sir !$(RESET)"

re: fclean all

.PHONY: all fclean clean re