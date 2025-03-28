include config.mk

# variables
NAME		:= ft_malcolm
CFLAGS		:= -Wall -Wextra -Werror -g
CC			:= cc

# source file variables
INC			:= ./inc
HEADER		:= $(INC)/ft_malcolm.h
DIR			:= ./srcs
SRCS		:= main.c aux_functions.c interface.c
OBJ			:= $(patsubst %.c, $(DIR)/%.o, $(SRCS))
TRUE		= 0

# dependencies
LIBFT_DIR	:= ./libft
LIBFT		:= $(LIBFT_DIR)/libft.a
LIBFT_HEADER:= $(LIBFT_DIR)/include

# build rules
all: $(LIBFT) $(NAME)

$(NAME): $(OBJ)
	$(CC) -I $(INC) -I $(LIBFT_HEADER) -o $@ $^ $(LIBFT)
	sudo ./$(NAME) $(ARG)

$(DIR)/%.o: $(DIR)/%.c $(HEADER)
	$(CC) $(CFLAGS) -I $(INC) -I $(LIBFT_HEADER) -c $< -o $@ -D VERBOSE=$(TRUE)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)
	@$(MAKE) fclean -s -C $(LIBFT_DIR)

re: fclean all

tcp:
	sudo tcpdump -n -e -tttt arp

$(LIBFT):
	@$(MAKE) all -s -C $(LIBFT_DIR)

bonus: TRUE=1
bonus: clean all
