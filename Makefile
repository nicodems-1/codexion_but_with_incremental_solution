CC = cc
CFLAGS = -g3 -Wall -Wextra -Werror
SRCS := srcs/main.c srcs/parsing.c srcs/initialization.c srcs/cleanup.c srcs/monitor.c srcs/compilation_coders.c srcs/helpers_tools.c
OBJS    = $(SRCS:.c=.o)
PTHREAD := -pthread
INCLUDE_DIRS := -I./include

make: $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) $(PTHREAD) -o codexion $(OBJS)

run: make
	./codexion 10 500 100 100 100 2 0 edf

clean:
	rm -f srcs/*.o

fclean:
	rm -f codexion *.o

valgrind: make
	valgrind  --leak-check=full --track-origins=yes  ./codexion 5 500 60 60 60 20 0 fifo  

hellgrind: make
	valgrind --tool=helgrind 	./codexion 10 2000000 100 100 100 10 0 fifo 

sanitize: make
	