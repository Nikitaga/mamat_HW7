# Standard variables
CC     = gcc
CCLINK = $(CC)
CFLAGS = -g -Wall -std=c99
EXEC   = libgrades.so
OBJS   = grades.c grades.h linked-list.h
RM     = rm -rf *.o *so *.exe  libgrades.so


$(EXEC): grades.o
	$(CCLINK) -shared grades.o -o libgrades.so -llinked-list -L.

grades.o: $(OBJS)
	$(CC) $(CFLAGS) -c -fpic grades.c

clean:
	$(RM)
