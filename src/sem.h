// changes: renamed n_proc to N_PROC
// moved struct to header file
// removed the -1 return value from sem_init on error because we don't anticipate errors from lecture notes
// changed if(!(count <= my_procnum)) to if(!(count <= N_PROC)) to make it easier to compile and my_procnum is uninitialized
// used sigemptyset instead of setting SIGUSR1_response to 0 in sem_init because SIGUSR1_response is not an int
// moved the return 1 all the way to the bottom of sem_try
// added "s->wait_list[my_procnum] = getpid();" to while loop in sem_wait
// commented  s->wait_list[my_procnum] = getpid(); in sem_init
// commented  sigprocmask(SIG_SETMASK, &oldset, NULL); //reset signal mask to original in sem_wait

#include "spinlock.h"
#define N_PROC 64

struct sem {
    volatile unsigned int semaphore; //protects the data_example
    volatile char semaphore_lock; //protects the semaphore integer 
    pid_t wait_list[N_PROC]; //keeps track of processes waiting 
    int sleep_list[N_PROC]; //flag array (0 = awake, 1 = asleep)
};

extern int my_procnum;

void sem_init(struct sem *s, int count);
int sem_try(struct sem *s);
void sem_wait(struct sem *s);
void sem_inc(struct sem *s);







// #include <stdio.h>
// #include <stdlib.h>
// #include "spinlock.h"

// struct sem {
//     int supply;
//     volatile char *lock;
//     int num_sleepers; 
//     int sleepers[N_PROC]; // stores PID of the sleeper in each index. If there is no sleeper, then element should be 0.
// };

// void sem_init(struct sem *s, int count);
// int sem_try(struct sem *s);
// void sem_wait(struct sem *s);
// void sem_inc(struct sem *s);