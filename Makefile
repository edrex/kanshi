CC ?= cc

all:
	$(CC) -o kanshi -Wall -lwayland-client kanshi.c
