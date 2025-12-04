CC = gcc
CFLAGS += -g

all: spinlock sem

spinlock: test_spinlock.c spinlock.c tas64.S
	${CC} ${CFLAGS} -o $@ $^

sem: test_sem.c sem.c spinlock.c tas64.S
	${CC} ${CFLAGS} -o $@ $^

.PHONY: clean all
clean:
	@rm -rvf spinlock sem *.o