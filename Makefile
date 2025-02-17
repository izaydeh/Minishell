NAME =minishell

SRCS =src/main.c src/free_and_exit.c src/execute/get_path.c src/builtin/ft_cd.c src/builtin/ft_echo.c src/builtin/env/old_pwd.c src/builtin/env/shell_level.c src/builtin/env/new_pwd.c src/builtin/env/ft_env.c src/builtin/ft_export.c src/builtin/ft_unset.c src/builtin/ft_pwd.c src/builtin/is_builtin_function.c src/execute/pipe.c src/signals/handle_signals.c src/execute/redirection.c src/utils.c src/expander/expand_tokens.c

OBJ =$(SRCS:.c=.o)
LIBFT_PATH=libft
CC =cc 
CFLAGS =-Wall -Wextra -Werror -g
LINKER =-L./libft -lft -lreadline
INC =-I./inc -I./libft/inc

all: LIB $(NAME)


LIB:
	make -C $(LIBFT_PATH)

$(NAME) : $(OBJ)
	$(CC) $^ $(LINKER) -o $@ 

%.o:%.c
	$(CC) $(CFLAGS) $(INC) $< -c -o $@

clean:
		rm -rf $(OBJ)
		make -C $(LIBFT_PATH) clean 
		

fclean: clean
		rm -rf $(NAME)
		make -C $(LIBFT_PATH) fclean
		

re: fclean all

.PHONY: all clean fclean re LIB
