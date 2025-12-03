#include <stdio.h>
#include <stdlib.h>

// read the lecture notes! it's not that deep (note to self)

struct sem {
    char data;
};

void sem_init(struct sem *s, int count);
void sem_try(struct sem *s);
void sem_wait(struct sem *s);
void sem_inc(struct sem *s);