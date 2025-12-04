#include "sem.h"

#define NUM_RESOURCES 5
int main() {
    struct sem semaphore;
    
    // create scarce semaphore resources
    sem_init(&semaphore, NUM_RESOURCES);
    
    int child_pid = 1;
    
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
        
    // every child process tries to get a semaphore
    if (child_pid == 0) {
        sem_wait(&semaphore); // every process tries to get a semaphore
        printf("Process with my_procnum %d got semaphore\n", my_procnum);
        sem_inc(&semaphore);
        printf("Process with my_procnum %d returned semaphore\n", my_procnum);
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
    
    return 0;
}