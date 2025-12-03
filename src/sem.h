#include <stdio.h>
#include <stdlib.h>
#include "spinlock.h"

struct sem {
    int supply;
    volatile char *lock;
    int num_sleepers; 
    int sleepers[N_PROC]; // stores PID of the sleeper in each index. If there is no sleeper, then element should be 0.
};

void sem_init(struct sem *s, int count);
int sem_try(struct sem *s);
void sem_wait(struct sem *s);
void sem_inc(struct sem *s);