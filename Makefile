CC = cc
CFLAGS = -g3 -Wall -Wextra -Werror
SRCS := main.c parsing2.c parsing.c initialization.c time_calculation.c cleanup.c actions_coder.c monitor.c
PTHREAD := -pthread
INCLUDE_DIRS := -I./include
OBJS    = $(SRCS:.c=.o)


make: $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) $(PTHREAD) -o codexion $(OBJS)

run: make
	./codexion 10 1100 100 100 300 2 100 fifo 

clean:
	rm -f *.o

fclean:
	rm -f codexion *.o

valgrind: make
	valgrind  --leak-check=full --track-origins=yes  ./codexion 5 500 60 60 60 20 0 fifo  

hellgrind: make
	valgrind --tool=helgrind 	./codexion 5 800 200 60 60 20 200 fifo  

sanitize: make
	