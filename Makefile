include args.mk
.PHONY: clean re fclean run insp bonus

# variables
NAME		:= ft_malcolm
CFLAGS		:= -Wall -Wextra -Werror -g
CC		:= cc

# source file variables
INC		:= ./inc
HEADER		:= $(INC)/ft_malcolm.h
DIR		:= ./srcs
SRCS		:= main.c aux_functions.c interface.c init.c broadcast.c
BONUS_SRC	:= hostname.c verbose_out.c
OBJ		:= $(patsubst %.c, $(DIR)/%.o, $(SRCS))
TRUE		= 0

# dependencies
LIBFT_DIR	:= ./libft
LIBFT		:= $(LIBFT_DIR)/libft.a
LIBFT_HEADER	:= $(LIBFT_DIR)/include

# build rules
all: $(LIBFT) $(NAME)

$(NAME): $(OBJ)
	$(CC) -I $(INC) -I $(LIBFT_HEADER) -o $@ $^ $(LIBFT)

$(DIR)/%.o: $(DIR)/%.c $(HEADER)
	$(CC) $(CFLAGS) -I $(INC) -I $(LIBFT_HEADER) -c $< -o $@ -D VERBOSE=$(TRUE)

clean:
	rm $(DIR)/*.o

fclean: clean
	rm -f $(NAME)
	@$(MAKE) fclean -s -C $(LIBFT_DIR)

re: fclean all

run: $(NAME)
	sudo ./$(NAME) $(ARG)

# inspect packages
insp:
	sudo tcpdump -i "$$(ip -o addr show | awk '/inet .* brd/ {print $$2}' | head -n1)" -vvv -S -xx arp

$(LIBFT):
	@$(MAKE) all -s -C $(LIBFT_DIR)


###	bonus section	###
bonus:
	$(MAKE) TRUE="1" SRCS="$(SRCS) $(BONUS_SRC)" all

# test bonus part
do:
	$(MAKE) TRUE="1" SRCS="$(SRCS) $(BONUS_SRC)" all
	sudo ./$(NAME) $(ARG)
