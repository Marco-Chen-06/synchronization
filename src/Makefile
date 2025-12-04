CC = gcc
CFLAGS += -g

all: spinlock sem

spinlock: spinlock.c tas64.S
	${CC} ${CFLAGS} -o $@ $^

sem: sem.c spinlock.c tas64.S
	${CC} ${CFLAGS} -o $@ $^

.PHONY: clean all
clean:
	@rm -rvf spinlock sem *.o