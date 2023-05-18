cddos: main.c
	$(CC) -o $@ $^ -O3 -DNDEBUG -s