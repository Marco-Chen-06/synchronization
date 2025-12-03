#include "sem.h"

void sem_init(struct sem *s, int count) {
    s->lock = 0;
    s->supply = count;
    for (int i = 0; i < N_PROC; i++) {
        s->sleepers[i] = 0;
    }
}

int sem_try(struct sem *s) {
    spin_lock(s->lock);
    // if "P" operation would block, return 0
    if (s->supply <= 0) {
        spin_unlock(s->lock);
        return 0;
    }
    spin_unlock(s->lock);
    return 1;
}

// unfinished
void sem_wait(struct sem *s) {
    spin_lock(s->lock);
}

void sem_inc(struct sem *s) {
    spin_lock(s->lock);

    // wake everyone up because supply went from 0 to 1
    if (s->supply == 0) {
        s->supply++;
        for (int i = 0; i < s->num_sleepers; i++) {
            // not sure if im allowed to put an integer into kill
            kill(s->sleepers[i], SIGUSR1);
        }
        s->num_sleepers = 0;
        spin_unlock(s->lock);
        return;
    }

    // in every other case just increment supply like normal
    s->supply++;
    spin_unlock(s->lock);
    return;
}



