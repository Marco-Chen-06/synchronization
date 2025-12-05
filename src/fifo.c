#include "fifo.h"
void fifo_init(struct myfifo *f) {

    // initialize memory for fifo and two semaphores in myfifo
    sem_init(&f->lock_sem, 1);
    sem_init(&f->count_sem, MYFIFO_BUFSIZ);
    
    // set everything to 0 because i think that's a good starting point for the buf
    for (int i = 0; i < MYFIFO_BUFSIZ; i++) {
        f->buf[i] = 0;
    }

    f->item_count = 0;
    f->next_write = 0;
    f->next_read = 0;
}

void fifo_wr(struct myfifo *f, unsigned long d) {
    sem_wait(&f->count_sem); // wait on line to write
    sem_wait(&f->lock_sem); // mutex lock the fifo
    f->buf[f->next_write++] = d;
    f->next_write %= MYFIFO_BUFSIZ; // circular buffer loopback behavior
    f->item_count++;
    sem_inc(&f->lock_sem); // unlock the fifo
}
unsigned long fifo_rd(struct myfifo *f) {
    unsigned long ret_val;
    sem_wait(&f->lock_sem); // mutex lock the fifo
    ret_val = f->buf[f->next_read++];
    f->next_read %= MYFIFO_BUFSIZ;
    f->item_count--;
    sem_inc(&f->lock_sem); // unlock the fifo
    sem_inc(&f->count_sem); // wait online to read
    return ret_val;
}