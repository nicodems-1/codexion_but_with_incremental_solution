CC = cc
CFLAGS = -Wall -Wextra -Werror
SRCS := main.c parsing2.c parsing.c initialization.c time_calculation.c cleanup.c actions_coder.c monitor.c
PTHREAD := -pthread
INCLUDE_DIRS := -I./include
OBJS    = $(SRCS:.c=.o)


make: $(OBJS)
	$(CC) $(INCLUDE_DIRS) $(PTHREAD) -o codexion $(OBJS)

run: make
	./codexion 5 1000 30 200 50 20 1 10 fifo 

clean:
	rm -f *.o

fclean:
	rm -f codexion *.o

valgrind: make
	valgrind --leak-check=full --track-origins=yes ./codexion 10 2000 3 4 5 6 7 8 fifo

hellgrind: make
	valgrind --tool=helgrind ./codexion 2 100 2 40 5 4 2 2 fifo

sanitize: make
	