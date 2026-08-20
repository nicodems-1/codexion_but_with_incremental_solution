ARGS = 10 500 100 100 100 2 0 edf
CC = cc
CFLAGS = -g3 -Wall -Wextra -Werror
SRCS := build/srcs/main.c \
		build/srcs/parsing.c \
		build/srcs/initialization.c \
		build/srcs/cleanup.c \
		build/srcs/monitor.c \
		build/srcs/compilation_coders.c \
		build/srcs/helpers_tools.c

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

valgrind: make
	valgrind  --leak-check=full --track-origins=yes  ./codexion $(ARGS) 

hellgrind: make
	valgrind --tool=helgrind 	./codexion $(ARGS)

sanitize: make
	