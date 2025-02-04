# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pracksaw <pracksaw@student.42bangkok.co    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/04 23:10:19 by pracksaw          #+#    #+#              #
#    Updated: 2025/02/04 23:48:11 by pracksaw         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

MAKEFLAGS += --no-print-directory

NAME			=	philo

################################################################################
## DIRECTORIES
DIR_INC			=	./inc/
DIR_SRC			=	./src/

INCLUDE			=	-I $(DIR_INC)

################################################################################
## COMPILATION & FLAGS
CC				=	cc
CFLAGS			=	-Wall -Wextra -Werror -g -O0
HEADERS			=	-I$(DIR_INC)
LIBRARIES       =   -lpthread
RM				=	rm -rf

################################################################################
## SOURCES
SRCS = \
        $(DIR_SRC)/philo.c \
        $(DIR_SRC)/ft_utils.c \
		$(DIR_SRC)/init_n_err.c \
		$(DIR_SRC)/threads.c \

OBJS			=	$(SRCS:.c=.o)
DEPS			=	$(OBJS:.o=.d)

TOTAL_FILES		=	$(words $(SRCS))
COMPILED_FILES	=	0
DOTS			=	$(shell printf ".%.0s" $(shell seq $(TOTAL_FILES)))

################################################################################
## RULES
all:				$(NAME)
					@echo "$(GREEN)Build complete! 🥰 $(NAME) has been created.$(NC)"

$(NAME):			$(OBJS)
					@$(CC) $(CFLAGS) $(OBJS) $(LIBRARIES) -o $(NAME)

%.o: %.c $(DIR_INC)philo.h
					@$(eval COMPILED_FILES=$(shell echo $$(($(COMPILED_FILES) + 1))))
					@DOTS_LEFT=$(shell echo "$(DOTS)" | cut -c$(COMPILED_FILES)-) ; \
					printf "\33[2K\r$(CYAN)Currently compiling: $< $(COMPILED_FILES)/$(TOTAL_FILES)$(NC) 💀%s"
					@$(CC) $(CFLAGS) $(HEADERS) -c $< -o $@
					@if [ $(COMPILED_FILES) -eq $(TOTAL_FILES) ]; then echo; fi

clean:
					@echo "$(YELLOW)Cleaning object files... 🧹$(NC)"
					@$(RM) $(OBJS) $(OBJS:.o=.d)
					@echo "\r$(GREEN)Object files cleaned! ✨$(NC)"

fclean:				clean
					@echo "$(YELLOW)Cleaning philo executable... 🧹$(NC)"
					@$(RM) $(NAME)
					@echo "\r$(GREEN)Executable cleaned! ✨$(NC)"

re:					fclean all

.PHONY:				all clean fclean re

################################################################################
## COLORS
GREEN = \033[0;32m
CYAN = \033[0;36m
YELLOW = \033[0;33m
RED = \033[0;31m
NC = \033[0m