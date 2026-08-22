NAME        = codexion
ARGS        = 10 300 100 100 100 2 0 fifo

CC          = cc
CFLAGS      = -g3 -Wall -Wextra -Werror
PTHREAD     = -pthread
INCLUDES    = -I./include

SRC_DIR     = build/srcs/
OBJ_DIR     = build/obj/

SRCS_FILES  = cleanup1.c \
              cleanup2.c \
              compilation_and_queue.c \
              dongle_operations.c \
              helpers_tools.c \
              initialization1.c \
              initialization2.c \
              main.c \
              monitor.c \
              parsing.c

SRCS        = $(addprefix $(SRC_DIR), $(SRCS_FILES))
OBJS        = $(addprefix $(OBJ_DIR), $(SRCS_FILES:.c=.o))


all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) $(PTHREAD) -o $(NAME) $(OBJS)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@


clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all


run: $(NAME)
	./$(NAME) $(ARGS)

valgrind: $(NAME)
	valgrind --leak-check=full --track-origins=yes --show-reachable=yes -s --show-leak-kinds=all ./$(NAME) $(ARGS) 

helgrind: $(NAME)
	valgrind --tool=helgrind ./$(NAME) $(ARGS)

.PHONY: all clean fclean re run valgrind helgrind
	