FILES := main.c utils.c simple_parsing.c create_coders.c

INCLUDE_DIRS := -I./include

make: $(FILES)
	$(CC) $(INCLUDE_DIRS) -o codexion $(FILES)