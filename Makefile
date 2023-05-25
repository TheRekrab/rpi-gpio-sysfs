CC=gcc
CFLAGS =-g -Wall -lc
BIN=test

all:$(BIN)

%:%.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f *.o a.out $(BIN)
