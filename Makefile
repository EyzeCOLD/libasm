NAME := libasm.a


SRCS	:= ft_strlen.s \
		   ft_strcpy.s \
		   ft_strcmp.s \
		   ft_write.s \
		   ft_read.s \
		   ft_strdup.s \
		   libasm.inc
SRC_DIR	:= ./src/
SRCS	:= $(addprefix $(SRC_DIR), $(SRCS))

OBJ_DIR	:= ./obj/
OBJS	:= $(patsubst $(SRC_DIR)%.s, $(OBJ_DIR)%.o, $(SRCS))

###

ASM			:= nasm
ASM_FLAGS	:= -f elf64 -g -F dwarf
AR			:= ar
AR_FLAGS	:= -rcs

###

TEST := libasm-tests

TEST_SRCS	:= tests.c
TEST_DIR	:= ./tests/
TEST_SRCS	:= $(addprefix $(TEST_DIR), $(TEST_SRCS))

CC			:= gcc
CFLAGS		:= -Wall -Wextra -Werror -g

###

all: $(NAME)

test: $(NAME)
	$(CC) $(CFLAGS) $(TEST_SRCS) $(NAME) -o $(TEST)
	./$(TEST)

$(NAME): $(OBJS)
	$(AR) $(AR_FLAGS) $(NAME) $?

$(OBJ_DIR)%.o: $(SRC_DIR)%.s
	@mkdir -p $(dir $@)
	$(ASM) $(ASM_FLAGS) $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean:
	@make clean
	rm -rf $(NAME)
	rm -rf $(TEST)

re:
	@make fclean
	@make all

.PHONY: all test clean fclean re
