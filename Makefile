ARGS = 10 300 100 100 100 2 0 fifo
CC = cc
CFLAGS = -g3 -Wall -Wextra -Werror
SRCS := $(wildcard build/srcs/*.c)

OBJS    = $(SRCS:build/srcs/%.c=build/obj/%.o)  
PTHREAD := -pthread
INCLUDE_DIRS := -I./include

all: $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) $(PTHREAD) -o codexion $(OBJS)

run: codexion
	./codexion $(ARGS)

build/obj/%.o: build/srcs/%.c
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -c $< -o $@

clean:
	rm -f build/srcs/*.o

fclean:
	rm -f codexion
	rm -f build/obj/*.o

valgrind: codexion
	valgrind  --leak-check=full --track-origins=yes  --show-reachable=yes -s --show-leak-kinds=all ./codexion $(ARGS) 

hellgrind: codexion
	valgrind --tool=helgrind 	./codexion $(ARGS)
	