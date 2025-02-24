NAME:= ft_malcolm
DIR:= ./srcs
SRCS:= main.c
OBJ= $(patsubst %.c,%.o,$(DIR)/$(SRCS))

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $@
	./$(NAME)

$(DIR)/%.o: $(DIR)/%.c
	$(CC) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
