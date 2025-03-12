include config.mk

# variables
CFLAGS:= -g -Wall -Wextra -Werror
NAME:= ft_malcolm
DIR:= ./srcs
LIBFT_DIR:= ./libft
LIBFT:=$(LIBFT_DIR)/libft.a
INC:=./inc
HEADER:=./inc/ft_malcolm.h
LIBFT_HEADER:=./libft/include
SRCS:= main.c aux_functions.c interface.c
OBJ= $(patsubst %.c, $(DIR)/%.o, $(SRCS))

# build rules
all: $(LIBFT) $(NAME)

$(NAME): $(OBJ)
	$(CC) -I $(INC) -I $(LIBFT_HEADER) -o $@ $^ $(LIBFT)
	sudo ./$(NAME) $(ARG)

$(DIR)/%.o: $(DIR)/%.c $(HEADER)
	$(CC) $(CFLAGS) -I $(INC) -I $(LIBFT_HEADER) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)
	@$(MAKE) fclean -s -C $(LIBFT_DIR)

re: fclean all

q:
	./$(NAME) $(ARG)

test:
	arp -a

tcp:
	sudo tcpdump -n -e -tttt arp

$(LIBFT):
	@$(MAKE) all -s -C $(LIBFT_DIR)
