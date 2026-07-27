CC = cc
CFLAGS = -Wall -Wextra -Werror
SRCS := main.c parsing2.c parsing.c create_coders.c time_calculation.c cleanup.c debug_refacto.c
PTHREAD := -pthread
INCLUDE_DIRS := -I./include
OBJS    = $(SRCS:.c=.o)


make: $(OBJS)
	$(CC) $(INCLUDE_DIRS) $(PTHREAD) -o codexion $(OBJS)

run: make
	./codexion 5 2 3 4000000 5 6 7 8 fifo

clean:
	rm -f *.o

fclean:
	rm -f codexion *.o

valgrind: make
	valgrind --leak-check=full ./codexion 1 2 3 4 5 6 7 8 fifo

hellgrind: make
	valgrind --tool=helgrind ./codexion 1 2 3 4 5 6 7 8 fifo

sanitize: make
	