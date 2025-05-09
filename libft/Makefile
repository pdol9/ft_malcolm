# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pdolinar <pdolinar@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/01/02 12:40:43 by pdolinar          #+#    #+#              #
#    Updated: 2022/09/18 23:37:50 by pdolinar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	:=		libft.a
INC		:=		./include
HEADER	:=		$(INC)/libft.h

CC		:=		gcc
CFLAGS	:=		-Wall -Wextra -Werror
RM		:=		rm -rf

SRC		:=		ft_atoi.c \
				ft_bzero.c \
				ft_calloc.c \
				ft_isalnum.c \
				ft_isalpha.c \
				ft_isascii.c \
				ft_isdigit.c \
				ft_isprint.c \
				ft_itoa.c \
				ft_memchr.c \
				ft_memcmp.c \
				ft_memcpy.c \
				ft_memmove.c \
				ft_memset.c \
				ft_putchar_fd.c \
				ft_putendl_fd.c \
				ft_putnbr_fd.c \
				ft_putstr_fd.c \
				ft_split.c \
				ft_strchr.c \
				ft_strdup.c \
				ft_striteri.c \
				ft_strjoin.c \
				ft_strlcat.c \
				ft_strlcpy.c \
				ft_strlen.c \
				ft_strmapi.c \
				ft_strncmp.c \
				ft_strnstr.c \
				ft_strrchr.c \
				ft_strtrim.c \
				ft_substr.c \
				ft_tolower.c \
				ft_toupper.c

SRC_BONUS	:=	ft_lstadd_back.c \
				ft_lstadd_front.c \
				ft_lstclear.c \
				ft_lstdelone.c \
				ft_lstiter.c \
				ft_lstlast.c \
				ft_lstmap.c \
				ft_lstnew.c \
				ft_lstsize.c

SRC_DIR		:=	src/
OBJ_DIR		:=	build/

OBJ := $(addprefix $(OBJ_DIR),$(notdir $(SRC:.c=.o)))
OBJ_BONUS := $(addprefix $(OBJ_DIR),$(notdir $(SRC_BONUS:.c=.o)))

all: $(OBJ_DIR) $(NAME)

$(NAME): $(OBJ)
	ar crs $@ $^
	@echo "$(NAME) compiled!"

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)%.o:$(SRC_DIR)%.c $(HEADER)
	$(CC) $(CC_FLAGS) -I$(INC) -c $< -o $@

bonus: all $(OBJ_BONUS)
	ar crs $(NAME) $(OBJ_BONUS)

clean:
	@$(RM) $(OBJ_DIR)
	@echo "All libft object files deleted"

fclean:	clean
	@$(RM) $(NAME)
	@echo "$(NAME) deleted"

norm:
	norminette $(SRC_DIR) $(INC)

re:	fclean all

.PHONY:	all bonus clean fclean re norm
