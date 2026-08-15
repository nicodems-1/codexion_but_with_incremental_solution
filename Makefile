CC = cc
CFLAGS = -g3 -Wall -Wextra -Werror
SRCS := main.c parsing2.c parsing.c initialization.c time_calculation.c cleanup.c actions_coder.c monitor.c helpers.c
PTHREAD := -pthread
INCLUDE_DIRS := -I./include
OBJS    = $(SRCS:.c=.o)


make: $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) $(PTHREAD) -o codexion $(OBJS)

run: make
	./codexion 10 1500 100 20 20 2 500 fifo 

clean:
	rm -f *.o

fclean:
	rm -f codexion *.o

valgrind: make
	valgrind  --leak-check=full --track-origins=yes  ./codexion 2 1000 100 200 200 5 100 fifo 

hellgrind: make
	valgrind --tool=helgrind 	./codexion 2 10 1 2 2 1 1 fifo 

sanitize: make
	