CC = gcc
CFLAGS += -g

spinlock : spinlock.c tas64.S 
		${CC} ${CFLAGS} -o $@ $^

.PHONY: clean
clean:
		@rm -rvf *.o
