CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

msh: msh.c
	$(CC) $(CFLAGS) -o msh msh.c

clean:
	rm -f msh
