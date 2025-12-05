CC = gcc
CFLAGS += -g

all: spinlock sem fifo

spinlock: test_spinlock.c spinlock.c tas64.S
	${CC} ${CFLAGS} -o $@ $^

sem: test_sem.c sem.c spinlock.c tas64.S
	${CC} ${CFLAGS} -o $@ $^

fifo: test_fifo.c fifo.c sem.c spinlock.c tas64.S

.PHONY: clean all
clean:
	@rm -rvf spinlock sem *.o