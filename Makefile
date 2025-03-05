NAME = minishell

SRCS = src/main.c \
       src/allocate/dir.c \
       src/allocate/operator.c \
       src/allocate/command.c \
       src/free_and_exit.c \
       src/signals/handle_signals.c \
       src/split/split_utils.c \
       src/split/split_input.c \
       src/split/check_operator.c \
       src/split/split_command.c \
       src/builtin/ft_exit.c \
       src/builtin/ft_cd.c \
       src/builtin/ft_echo.c \
       src/builtin/env/old_pwd.c \
       src/builtin/env/shell_level.c \
       src/builtin/env/new_pwd.c \
       src/builtin/env/ft_env.c \
       src/builtin/ft_export.c \
       src/builtin/ft_unset.c \
       src/builtin/ft_pwd.c \
       src/builtin/is_builtin_function.c \
       src/execute/pipe.c \
       src/execute/get_path.c \
       src/execute/check_dir.c \
       src/execute/redirection.c \
       src/expander/expand_tokens.c \
       src/expander/expand_tokens_two.c \
       src/expander/process_dollar_sign.c

OBJ_DIR = obj
OBJ = $(SRCS:src/%.c=$(OBJ_DIR)/%.o)
LIBFT_PATH = libft
CC = cc 
CFLAGS = -Wall -Wextra -Werror -g
LINKER = -L./libft -lft -lreadline
INC = -I./inc -I./libft/inc

all: LIB $(NAME)

LIB:
	make -C $(LIBFT_PATH)

$(NAME): $(OBJ)
	$(CC) $^ $(LINKER) -o $@

$(OBJ_DIR)/%.o: src/%.c | $(OBJ_DIR)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)
	make -C $(LIBFT_PATH) clean 

fclean: clean
	rm -rf $(NAME)
	make -C $(LIBFT_PATH) fclean

re: fclean all

.PHONY: all clean fclean re LIB