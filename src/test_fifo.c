#include "fifo.h"

int main() {
struct myfifo *f = mmap(NULL, sizeof(struct myfifo) + sizeof(struct sem) * 2, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);    
    int child_pid = 1;
    fifo_init(f);

    // fork a N_PROC processes
    for(int i = 0; i < N_PROC; i++) {
        if(child_pid == 0) {
            continue;
        }
        child_pid = fork();
        if(child_pid == 0) {
            my_procnum = i + 1; 
        }
    }
    
    if(child_pid != 0) {
        my_procnum = 0; 
    }
        
    // every child process with my_procnum < 10 waits for resources
    if ((child_pid == 0) && (my_procnum < 10)){
        for (int i = 0; i < 1000; i++) {
        fifo_wr(f, 1);
        printf("Process with my_procnum %d wrote 1 to fifo\n", my_procnum);
        // usleep(100000); // process hoards semaphore for 1000000 microseconds (0.1 ms)
        }
        
    }

    // every child process with my_procnum >= 10 trys to get resource
    if ((child_pid == 0) && (my_procnum >= 10)) {
        fifo_wr(f, 2);
        printf("Process with my_procnum %d wrote 2 to fifo\n", my_procnum);
    }


    // children exit here
    if(child_pid == 0) {
        exit(EXIT_SUCCESS);
    }
    
    // parent waits for all children
    while(wait(NULL) > 0);
    
    // // Parent prints result
    // if(child_pid != 0) {
    //     fprintf(stdout, "Counter: %lld (expected: 10000000)\n", *counter);
    //     if(*counter == 10000000) {
    //         fprintf(stdout, "SUCCESS!\n");
    //     } else {
    //         fprintf(stdout, "FAILURE - race condition detected\n");
    //     }
    // }
    

    // while (f->item_count > 0) {
    //     printf("Found %d in fifo\n", fifo_rd(f));
    // }
    return 0;
}