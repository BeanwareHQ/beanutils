CC = cc
CAT = /usr/bin/cat
CFLAGS = -Wall -Wpedantic -O2 
OBJS = string.o io.o logger.o array.o

files = beanutils/string.c beanutils/io.c beanutils/logger.c beanutils/array.c


build: $(files)
	$(CAT) $(files) > beanutils/beanutils.c
	$(CC) $(CFLAGS) -c beanutils/beanutils.c
	rm beanutils/beanutils.c

clean:
	rm -f *.o
