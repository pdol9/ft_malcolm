include config.mk

# variables
NAME:= ft_malcolm
DIR:= ./srcs
LIBFT_DIR:= ./libft
LIBFT:=$(LIBFT_DIR)/libft.a
INC:=./inc
HEADER:=./inc/malcolm.h
LIBFT_HEADER:=./libft/include
SRCS:= main.c
OBJ= $(patsubst %.c,%.o,$(DIR)/$(SRCS))

# colors
CLEAR = \033[0m
PR = \033[1;35m
GR = \033[1;32m

# build rules
all: $(NAME)
#	@echo "Given parameters are: " $(ARG)

$(NAME): $(LIBFT) $(OBJ)
	$(CC) -I $(INC) -I $(LIBFT_HEADER) $(LIBFT) $(OBJ) -o $@
	./$(NAME) $(ARG)

$(DIR)/%.o: $(DIR)/%.c $(HEADER)
	$(CC) -c $< -o $@ -I $(INC) -I $(LIBFT_HEADER)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

test:
	arp -a

$(LIBFT):
	@$(MAKE) all -s -C $(LIBFT_DIR)
