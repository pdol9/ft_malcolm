include args.mk
.PHONY: clean re fclean run insp bonus

# compile constants
NAME		:= ft_malcolm
CC		:= cc

# compile variable
CFLAGS		+= -Wall -Wextra -Werror $(GDB) -DVERBOSE=$(VERBOSE) -DDEBUG=$(DEBUG)

# compiling input/output file constants
INC		:= ./inc
HEADER		:= $(INC)/ft_malcolm.h
DIR		:= ./srcs
SRCS		:= verbose_out.c aux_functions.c init.c interface.c broadcast.c main.c
BONUS_SRC	:= LAN_info.c hostname.c
OBJ		:= $(patsubst %.c, $(DIR)/%.o, $(SRCS))
BONUS_OBJ	:= $(patsubst %.c, $(DIR)/%.o, $(BONUS_SRC))

# enabling C preprocessor macros to compile additional features
VERBOSE		?= 0
DEBUG		?= 0

GDB		?=

# dependencies
LIBFT_DIR	:= ./libft
LIBFT		:= $(LIBFT_DIR)/libft.a
LIBFT_HEADER	:= $(LIBFT_DIR)/include

# build rules
# ------------------------------------------------------------- #
all: $(LIBFT) $(NAME)

$(NAME): $(OBJ)
	$(CC) -I $(INC) -I $(LIBFT_HEADER) -o $@ $^ $(LIBFT)

$(DIR)/%.o: $(DIR)/%.c $(HEADER)
	$(CC) $(CFLAGS) -I $(INC) -I $(LIBFT_HEADER) -c $< -o $@

clean:
	rm $(OBJ) $(BONUS_OBJ) || true

fclean: clean
	rm -f $(NAME)
	@$(MAKE) fclean -s -C $(LIBFT_DIR)

re: fclean all

$(LIBFT):
	@$(MAKE) all -s -C $(LIBFT_DIR)

# debug / inspect network information
# ------------------------------------------------------------- #

insp:
	sudo tcpdump -i "$$(ip -o addr show | awk '/inet .* brd/ {print $$2}' | head -n1)" -vvv -S -xx arp

# bonus section
# ------------------------------------------------------------- #

bonus:
	$(MAKE) VERBOSE="1" SRCS="$(SRCS) $(BONUS_SRC)" all

# testing (bonus) part
# ------------------------------------------------------------- #

run: all
	sudo ./$(NAME) $(ARG)

# test with debugging enabled
do:	clean 
	$(MAKE) VERBOSE="1" DEBUG="1" GDB="-g" SRCS="$(SRCS) $(BONUS_SRC)" all
	sudo ./$(NAME) $(ARG)
