CC = cc
CFLAGS = -Wall -Wextra -Werror
SRCS := main.c parsing2.c parsing.c initialization.c time_calculation.c cleanup.c actions_coder.c monitor.c
PTHREAD := -pthread
INCLUDE_DIRS := -I./include
OBJS    = $(SRCS:.c=.o)


make: $(OBJS)
	$(CC) $(INCLUDE_DIRS) $(PTHREAD) -o codexion $(OBJS)

run: make
	./codexion 4 3000 3 400 5 10 7 2 fifo 

clean:
	rm -f *.o

fclean:
	rm -f codexion *.o

valgrind: make
	valgrind --leak-check=full ./codexion 1 2 3 4 5 6 7 8 fifo

hellgrind: make
	valgrind --tool=helgrind ./codexion 4 3000 3 400 5 4 70 2 fifo

sanitize: make
	